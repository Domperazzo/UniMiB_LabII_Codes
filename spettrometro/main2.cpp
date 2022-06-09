/*
	c++ -o main2 main2.cpp `root-config --glibs --cflags`
*/

#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "TGraph.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TMath.h"
#include "TLatex.h"

int main(int argc , char** argv) {

	gStyle->SetOptFit(1112);

//--------DETERMINAZIONE PARAMETRI LEGGE CAUCHY----------------------
  std::ifstream dati;
	dati.open("rifrazione_prisma.txt", std::ios::in);

  std::vector<double> v_indici, v_err, v_lambda;
  while (true) {
      double indice_rifrazione, err_indice_rifrazione, lambda;
	    dati >> indice_rifrazione;
	    dati >> err_indice_rifrazione;
			dati >> lambda;
      if(dati.eof()==true) break;
      v_indici.push_back(indice_rifrazione);
      v_err.push_back(err_indice_rifrazione);
			v_lambda.push_back(lambda);
  }
	dati.close();

  TGraphErrors grafico;
  for (int i = 0; i < 5; i++) {
      grafico.SetPoint(i, 1.0/(v_lambda.at(i)*v_lambda.at(i)), v_indici.at(i));
      grafico.SetPointError(i, 0, v_err.at(i));
  }
	grafico.SetTitle("Legge di Cauchy");
	grafico.GetHistogram()-> GetXaxis()->SetTitle("inverso del quadrato della l'unghezza d'onda [amstrong(-2)]");
	grafico.GetHistogram()-> GetYaxis()->SetTitle("indice di rifrazione");


  TF1 modello ("modello", "[0]*x+[1]");
	TFitResultPtr fit_result = grafico.Fit(&modello, "S");
	modello.SetParName(0, "B");
	modello.SetParName(1, "A");


	double A, B, sigma_A, sigma_B, cov_AB;
	A=modello.GetParameter(0);
	sigma_A = modello.GetParError(0);
	B=modello.GetParameter(1);
	sigma_B = modello.GetParError(1);
	cov_AB = fit_result->CovMatrix(1,1);
	std::cout << "cov_AB:\t" << cov_AB << '\n';
/*
	TApplication app ("app", &argc , argv);
  TCanvas c1;
  grafico.SetMarkerStyle(20);
  grafico.SetMarkerSize(0.5);
  grafico.Draw("AP");
	app.Run();
	c1.Print("legge di cauchy Hg.png","png");
*/

//----GAS INCOGNITO 2--------------------------------

	std::fstream f_GasIgnoto2;
	f_GasIgnoto2.open("gas_ignoto2.txt", std::ios::in);
	std::vector<double> v_indici_ignoti2, v_err_ignoti2;
	while (true) {
			double indice_rifrazione_ignoto2, err_indice_rifrazione_ignoto2;
			f_GasIgnoto2 >> indice_rifrazione_ignoto2;
			f_GasIgnoto2 >> err_indice_rifrazione_ignoto2;
			if(f_GasIgnoto2.eof()==true) break;
			v_indici_ignoti2.push_back(indice_rifrazione_ignoto2);
			v_err_ignoti2.push_back(err_indice_rifrazione_ignoto2);
		}
	f_GasIgnoto2.close();



//----FILE CONFRONTO 2 SODIO-------
	std::ifstream f_sodio;
	f_sodio.open("lambda_sodio.txt", std::ios::in);
	std::vector<double> v_lambdaSodio;
	while (true) {
		double lambdaSodio;
		f_sodio >> lambdaSodio;
		if (f_sodio.eof()==true) break;
		v_lambdaSodio.push_back(lambdaSodio);
	}



	std::vector<double>v_lambda_sperimentali2, v_SigmaLambda_sperimentali2;


	for (int i = 0; i < v_indici_ignoti2.size(); i++) {
		v_lambda_sperimentali2.push_back(sqrt(A/(v_indici_ignoti2.at(i)-B)));
		std::cout << v_lambda_sperimentali2.at(i) << '\n';
	}



	for (size_t i = 0; i < v_indici_ignoti2.size(); i++) {
		v_SigmaLambda_sperimentali2.push_back(
																		-50*((1/(2*(v_indici_ignoti2.at(i)-A)))*sqrt(
																			v_indici_ignoti2.at(i)*v_indici_ignoti2.at(i)*(sigma_A*sigma_A
																		+ v_err_ignoti2.at(i)*v_err_ignoti2.at(i)) + 2*cov_AB*cov_AB
																		+ sigma_B*sigma_B/v_lambda_sperimentali2.at(i)/v_lambda_sperimentali2.at(i)))
																	);
	}

	TGraphErrors grafico_sperimentale2;
	for (int i = 0; i < v_lambda_sperimentali2.size(); i++) {
		grafico_sperimentale2.SetPoint(i, i+1, v_lambda_sperimentali2.at(i));
		grafico_sperimentale2.SetPointError(i, 0, 20*v_SigmaLambda_sperimentali2.at(i));
	}


	TGraph grafico_Sodio;


	for (int i = 0; i < v_lambda_sperimentali2.size(); i++) {
		grafico_Sodio.SetPoint(i, i+1, v_lambdaSodio.at(i));
	}


	grafico_sperimentale2.SetMarkerStyle(20);
	grafico_sperimentale2.SetMarkerSize(0.5);
	grafico_sperimentale2.SetMarkerColor(kBlue);
	grafico_sperimentale2.SetTitle("Spettro misurato");

	grafico_Sodio.SetMarkerStyle(20) ;
	grafico_Sodio.SetMarkerSize(0.5);
	grafico_Sodio.SetMarkerColor(kRed);
	grafico_Sodio.SetTitle("Spettro atteso");

//---MULTIGRAPH CONFRONTO 2
	TMultiGraph multi_Sodio2;

	//--Confronto Sodio
	multi_Sodio2.Add(&grafico_sperimentale2);
	multi_Sodio2.Add(&grafico_Sodio);
	TApplication myApp("myApp", &argc, argv);
	TCanvas c6;
	multi_Sodio2.Draw("AP");
	multi_Sodio2.SetTitle("Confronto con lo spettro di Na ; ordine di misurazione ; lunghezza d'onda");
	c6.BuildLegend();
	myApp.Run();
	c6.Print("confronto sodio.pdf", "pdf");


  return 0;
}
