/*
	c++ -o main main.cpp `root-config --glibs --cflags`
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
//	std::cout << "cov_AB:\t" << cov_AB << '\n';
/*
	TApplication app ("app", &argc , argv);
  TCanvas c1;
  grafico.SetMarkerStyle(20);
  grafico.SetMarkerSize(0.5);
  grafico.Draw("AP");
	app.Run();
	c1.Print("legge di cauchy Hg.png","png");
*/

//----GAS INCOGNITO 1------------------------------

  std::ifstream f_GasIgnoto;
	f_GasIgnoto.open("gas_ignoto1.txt", std::ios::in);

  std::vector<double> v_indici_ignoti, v_err_ignoti;
  while (true) {
      double indice_rifrazione_ignoto, err_indice_rifrazione_ignoto;
	    f_GasIgnoto >> indice_rifrazione_ignoto;
	    f_GasIgnoto >> err_indice_rifrazione_ignoto;
      if(f_GasIgnoto.eof()==true) break;
      v_indici_ignoti.push_back(indice_rifrazione_ignoto);
      v_err_ignoti.push_back(err_indice_rifrazione_ignoto);
  }
	f_GasIgnoto.close();

//----FILE CONFRONTO ELIO-------
	std::ifstream f_He;
	f_He.open("lambda_He.txt", std::ios::in);
	std::vector<double> v_lambdaElio;
	while (true) {
		double lambdaElio;
		f_He >> lambdaElio;
		if (f_He.eof()==true) break;
		v_lambdaElio.push_back(lambdaElio);
	}



	std::vector<double> v_lambda_sperimentali, v_SigmaLambda_sperimentali;

	for (int i = 0; i < v_indici_ignoti.size(); i++) {
		v_lambda_sperimentali.push_back(sqrt(A/(v_indici_ignoti.at(i)-B)));
		std::cout << v_lambda_sperimentali.at(i) << '\n';
	}

	for (size_t i = 0; i < v_lambda_sperimentali.size(); i++) {
		v_SigmaLambda_sperimentali.push_back(
																		-50*((1/(2*(v_indici_ignoti.at(i)-A)))*sqrt(
																			v_indici_ignoti.at(i)*v_indici_ignoti.at(i)*(sigma_A*sigma_A
																		+ v_err_ignoti.at(i)*v_err_ignoti.at(i)) + 2*cov_AB*cov_AB
																		+ sigma_B*sigma_B/v_lambda_sperimentali.at(i)/v_lambda_sperimentali.at(i)))
																	);
	}

	TGraphErrors grafico_sperimentale;
	for (int i = 0; i < v_lambda_sperimentali.size()-2; i++) {
			grafico_sperimentale.SetPoint(i, i+1, v_lambda_sperimentali.at(i));
			grafico_sperimentale.SetPointError(i, 0, 30*v_SigmaLambda_sperimentali.at(i));
	}




	TGraph grafico_Elio;

	for (int i = 0; i < v_indici_ignoti.size()-2; i++) {
		grafico_Elio.SetPoint(i, i+1, v_lambdaElio.at(i));
	}


	grafico_sperimentale.SetMarkerStyle(20);
	grafico_sperimentale.SetMarkerSize(0.5);
	grafico_sperimentale.SetMarkerColor(kBlue);
	grafico_sperimentale.SetTitle("Spettro misurato");

	grafico_Elio.SetMarkerStyle(20) ;
	grafico_Elio.SetMarkerSize(0.5);
	grafico_Elio.SetMarkerColor(kRed);
	grafico_Elio.SetTitle("Spettro atteso");

//---MULTIGRAPH CONFRONTO 1
	TMultiGraph multi_Elio;

	//--Confronto Elio
	multi_Elio.Add(&grafico_sperimentale);
	multi_Elio.Add(&grafico_Elio);


	TApplication myApp("myApp", &argc , argv);
	TCanvas c2;
	multi_Elio.Draw("AP");
	multi_Elio.SetTitle("Confronto con lo spettro di He ; ordine di misurazione ; lunghezza d'onda");
	c2.BuildLegend();
	myApp.Run();
	c2.Print("confronto tra lambda 4.pdf", "pdf");



  return 0;
}
