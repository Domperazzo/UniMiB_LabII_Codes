/*
c++ -o legge_cauchy legge_cauchy.cpp `root-config --glibs --cflags`
*/
#include<stdlib.h>
#include<vector>
#include<fstream>
#include<cmath>
#include<iostream>
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TStyle.h"

using namespace std;

double retta (double* x, double* p) {			//definizione della funzione per il fit (le variabili devono essere puntatori)

	return p[0] + x[0]*p[1] ;
}

double media ( vector<double> v ) {

	double somma = 0 ;

	for ( int i = 0 ; i < v.size() ; i++ ) {

		somma += v.at(i) ;
	}

	return somma / v.size() ;
}

double sigma ( vector<double> v ) {

	double somma = 0 , sommaq = 0 ;

	for ( int i = 0 ; i < v.size() ; i++ ) {

		somma += v.at(i);
		sommaq += v.at(i)*v.at(i);
	}

	return sqrt( (sommaq/v.size() - somma*somma/v.size()/v.size())*(static_cast<double>(v.size())/(v.size()-1)) ) ;
}

int main ( int argc , char** argv) {

	double deltaN, pressione;
	vector<double> lambda;
	vector<double> theta_mis;
	vector<double> theta_true;
	vector<double> sigma_theta;

	TApplication * theApp = new TApplication("theApp", &argc, argv);
    gStyle->SetOptFit(1112);
    ifstream f;
    f.open ("aria.txt", std::ios::in);

    vector<double> v_deltaN, v_deltaP;
    while (true)
      {
        double delta_N, delta_P;
        f >> delta_N;
        f >> delta_P;
        if (f.eof () == true) break;
        v_deltaN.push_back (delta_N);
        v_deltaP.push_back (delta_P);
      }
    f.close () ;


	vector<double> indici_rifrazione ;		//punti del grafico
	vector<double> sigma_indici;
	double lambda=632.8;//nanometri
	TGraphErrors grafico;

	for (int i = 0 ; i < 4 ; i++ ) {

		indici_rifrazione.push_back((v_deltaN.at(i)*lambda)/(2*v_deltaP.at(i)));
		sigma_indici.push_back( sigma_theta.at(i)*cos((theta_true.at(i) + 1.0498)/2)/ sin(1.0498/2) / 2) ;

		grafico.SetPoint( i, 1.0/(lambda.at(i)*lambda.at(i)), indici_rifrazione.at(i) ) ;
		grafico.SetPointError( i , 0 , sigma_indici.at(i) ) ;

	}

	grafico.SetMarkerStyle(20);	//forma dei punti sul grafico
	grafico.SetMarkerSize(0.5);

	TF1 modello ( "modello" , retta, 0, 1 , 2 ) ;

	TFitResultPtr fit_result = grafico.Fit( &modello , "SQ" );
	cout<<"Chi quadro: "<<fit_result->Chi2()<<endl;
	cout<<"Gradi di libertà: "<<fit_result->Ndf()<<endl;
	cout<<"Probabilità: "<<(fit_result->Prob())*100<<"%"<<endl;

	TApplication app ("app", &argc , argv);
	TCanvas c1;
	grafico.Draw("AP");
	grafico.GetHistogram()-> GetXaxis()->SetTitle("inverso del quadrato della l'unghezza d'onda");
	grafico.GetHistogram()-> GetYaxis()->SetTitle("indice di rifrazione");
	grafico.SetTitle("Legge di Cauchy");
	app.Run();
	c1.Print("legge di cauchy Hg.png","png");


	double A,B,sigma_A,sigma_B,cov_AB;

	A = modello.GetParameter(0);
	sigma_A = modello.GetParError(0);
	B = modello.GetParameter(1);
	sigma_B = modello.GetParError(1);
	cov_AB = fit_result->CovMatrix(1,0);
	

	ifstream dati_in;
	dati_in.open("gas_incognito.txt", ios::in) ;

	double theta0_in;			//lettura dati incogniti
	vector<double> theta_misin;
	vector<double> theta_in;
	vector<double> sigma_theta_in;

	dati_in >> theta0_in;
	dati_in >> val;

	theta0_in = (theta0_in + val/60) * M_PI/180;

	for( int i = 0 ; i < 3 ; i++ ) {

		for( int j = 0 ; j < 3 ; j++ ) {

			dati_in >> val;
			theta_misin.push_back(val);
			dati_in>>val;
			theta_misin.at(j) = (theta_misin.at(j) + val/60) * M_PI/180 ;
		}

		theta_in.push_back(media(theta_misin)-theta0_in);
		sigma_theta_in.push_back(sqrt( sigma(theta_misin)*sigma(theta_misin)/3 + 8.4616*pow(10,-8)));

		theta_misin.clear();
	}

	dati_in.close();

	double n_in ;
	double sigma_nin;
	double lambdain, sigma_lambdain;

	TGraphErrors lambda_sperimentali;
	TGraph lambda_ideali;

	for (int i = 0 ; i < 3 ; i++ ) {

		n_in = sin((theta_in.at(i) + 1.0498)/2) / sin(1.0498/2)  ;
		sigma_nin = sigma_theta_in.at(i)*cos((theta_in.at(i) + 1.0498)/2)/ sin(1.0498/2) / 2;

		lambdain = sqrt( B / (n_in - A) ) ;
		sigma_lambdain = (1/(2*(n_in-A)))*sqrt( lambdain*lambdain*(sigma_A*sigma_A + sigma_nin*sigma_nin) + 2*cov_AB*cov_AB + sigma_B*sigma_B/lambdain/lambdain) ;

		lambda_sperimentali.SetPoint( i , i+1 , lambdain ) ;
		lambda_sperimentali.SetPointError( i , 0 , sigma_lambdain ) ;

		cout<<"lambda "<<i<<": "<<lambdain<<" +- "<<sigma_lambdain<<endl;
	}

	lambda_ideali.SetPoint( 0 , 1 , 5875.6148 ) ;
	lambda_ideali.SetPoint( 1 , 2 , 5015.678 ) ;
	lambda_ideali.SetPoint( 2 , 3 , 4471.479 ) ;

	lambda_sperimentali.SetMarkerStyle(20);
	lambda_sperimentali.SetMarkerSize(0.5);
	lambda_sperimentali.SetTitle("spettro misurato");

	lambda_ideali.SetMarkerStyle(20) ;
	lambda_ideali.SetMarkerSize(0.5);
	lambda_ideali.SetMarkerColor(kRed);
	lambda_ideali.SetTitle("spettro atteso");

	TMultiGraph confronto;

	confronto.Add(&lambda_sperimentali);
	confronto.Add(&lambda_ideali);

	TCanvas c2;

	confronto.Draw("AP");
	confronto.SetTitle("Confronto con lo spettro di He ; ordine di misurazione ; lunghezza d'onda");
	c2.BuildLegend();
	c2.Print("confronto tra lambda.pdf", "pdf" );


	return 0;
}
