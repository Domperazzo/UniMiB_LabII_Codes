/*
	c++ -o leggeCauchy leggeCauchy.cpp `root-config --glibs --cflags`
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
	grafico.GetHistogram()-> GetXaxis()->SetTitle("inverso del quadrato della l'unghezza d'onda [#AA(-2)]");
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

	TApplication app ("app", &argc , argv);
  TCanvas c1;
  grafico.SetMarkerStyle(20);
  grafico.SetMarkerSize(0.5);
  grafico.Draw("AP");
	app.Run();
	c1.Print("legge di cauchy Hg.png","png");


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
	f_GasIgnoto.close();

//----FILE CONFRONTO 1 ARGON-----------
	std::ifstream f_confronto1;
	f_confronto1.open("lambda_Argon.txt", std::ios::in);

	std::vector<double> v_lambda1;
	while (true) {
			double lambda1;
			f_confronto1 >> lambda1;
			if(f_confronto1.eof()==true) break;
			v_lambda1.push_back(lambda1);
	}
	f_confronto1.close();


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


//----FILE CONFRONTO 3 MERCURIO-------
	std::ifstream f_Hg;
	f_Hg.open("lambda_Hg.txt", std::ios::in);
	std::vector<double> v_lambdaMercurio;
	while (true) {
		double lambdaMercurio;
		f_Hg >> lambdaMercurio;
		if (f_Hg.eof()==true) break;
		v_lambdaMercurio.push_back(lambdaMercurio);
	}

//----FILE CONFRONTO 4 ELIO-------
	std::ifstream f_He;
	f_He.open("lambda_Hg.txt", std::ios::in);
	std::vector<double> v_lambdaElio;
	while (true) {
		double lambdaElio;
		f_He >> lambdaElio;
		if (f_He.eof()==true) break;
		v_lambdaElio.push_back(lambdaElio);
	}



	std::vector<double> v_lambda_sperimentali, v_SigmaLambda_sperimentali, v_lambda_sperimentali2, v_SigmaLambda_sperimentali2;

	for (int i = 0; i < v_indici_ignoti.size(); i++) {
		v_lambda_sperimentali.push_back(sqrt(A/(v_indici_ignoti.at(i)-B)));
		std::cout << v_lambda_sperimentali.at(i) << '\n';
	}

	for (int i = 0; i < v_indici_ignoti2.size(); i++) {
		v_lambda_sperimentali2.push_back(sqrt(A/(v_indici_ignoti2.at(i)-B)));
	}

	for (size_t i = 0; i < v_lambda_sperimentali.size(); i++) {
		v_SigmaLambda_sperimentali.push_back(
																		-50*((1/(2*(v_indici_ignoti.at(i)-A)))*sqrt(
																			v_indici_ignoti.at(i)*v_indici_ignoti.at(i)*(sigma_A*sigma_A
																		+ v_err_ignoti.at(i)*v_err_ignoti.at(i)) + 2*cov_AB*cov_AB
																		+ sigma_B*sigma_B/v_lambda_sperimentali.at(i)/v_lambda_sperimentali.at(i)))
																	);
	}

	for (size_t i = 0; i < v_lambda_sperimentali2.size(); i++) {
		v_SigmaLambda_sperimentali2.push_back(
																		-50*((1/(2*(v_indici_ignoti2.at(i)-A)))*sqrt(
																			v_indici_ignoti2.at(i)*v_indici_ignoti2.at(i)*(sigma_A*sigma_A
																		+ v_err_ignoti2.at(i)*v_err_ignoti2.at(i)) + 2*cov_AB*cov_AB
																		+ sigma_B*sigma_B/v_lambda_sperimentali2.at(i)/v_lambda_sperimentali2.at(i)))
																	);
	}


	TGraphErrors grafico_sperimentale;
	for (int i = 0; i < v_lambda_sperimentali.size()-2; i++) {
			grafico_sperimentale.SetPoint(i, i+1, v_lambda_sperimentali.at(i));
			grafico_sperimentale.SetPointError(i, 0, 100*v_SigmaLambda_sperimentali.at(i));
	}

	TGraphErrors grafico_sperimentale2;
	for (int i = 0; i < v_lambda_sperimentali2.size(); i++) {
		grafico_sperimentale2.SetPoint(i, i+1, v_lambda_sperimentali2.at(i));
		grafico_sperimentale2.SetPointError(i, 0, 100*v_SigmaLambda_sperimentali2.at(i));
	}


	TGraph grafico_Argon, grafico_Sodio, grafico_Mercurio, grafico_Elio;

	for (int i = 0; i < v_indici_ignoti.size(); i++) {
		grafico_Argon.SetPoint(i, i+1, v_lambda1.at(i));
	}

	for (int i = 0; i < v_indici_ignoti.size(); i++) {
		grafico_Sodio.SetPoint(i, i+1, v_lambdaSodio.at(i));
	}

	for (int i = 0; i < v_indici_ignoti.size()-2; i++) {
		grafico_Mercurio.SetPoint(i, i+1, v_lambdaMercurio.at(i));
	}

	for (int i = 0; i < v_indici_ignoti.size()-2; i++) {
		grafico_Elio.SetPoint(i, i+1, v_lambdaElio.at(i));
	}
	grafico_Elio.RemovePoint(3);

	grafico_sperimentale.SetMarkerStyle(20);
	grafico_sperimentale.SetMarkerSize(0.5);
	grafico_sperimentale.SetMarkerColor(kBlue);
	grafico_sperimentale.SetTitle("Spettro misurato");

	grafico_sperimentale2.SetMarkerStyle(20);
	grafico_sperimentale2.SetMarkerSize(0.5);
	grafico_sperimentale2.SetMarkerColor(kBlue);
	grafico_sperimentale2.SetTitle("Spettro misurato");

	grafico_Argon.SetMarkerStyle(20) ;
	grafico_Argon.SetMarkerSize(0.5);
	grafico_Argon.SetMarkerColor(kRed);
	grafico_Argon.SetTitle("Spettro atteso");

	grafico_Sodio.SetMarkerStyle(20) ;
	grafico_Sodio.SetMarkerSize(0.5);
	grafico_Sodio.SetMarkerColor(kRed);
	grafico_Sodio.SetTitle("Spettro atteso");

	grafico_Mercurio.SetMarkerStyle(20) ;
	grafico_Mercurio.SetMarkerSize(0.5);
	grafico_Mercurio.SetMarkerColor(kRed );
	grafico_Mercurio.SetTitle("Spettro atteso");

	grafico_Elio.SetMarkerStyle(20) ;
	grafico_Elio.SetMarkerSize(0.5);
	grafico_Elio.SetMarkerColor(kRed);
	grafico_Elio.SetTitle("Spettro atteso");

//---MULTIGRAPH CONFRONTO 1
	TMultiGraph multi_Argon, multi_Sodio, multi_Mercurio, multi_Elio;
	//--Confronto Argon
	multi_Argon.Add(&grafico_sperimentale);
	multi_Argon.Add(&grafico_Argon);

	//--Confronto Sodio
	multi_Sodio.Add(&grafico_sperimentale);
	multi_Sodio.Add(&grafico_Sodio);

	//--Confronto Mercurio
	multi_Mercurio.Add(&grafico_sperimentale);
	multi_Mercurio.Add(&grafico_Mercurio);

	//--Confronto Elio
	multi_Elio.Add(&grafico_sperimentale);
	multi_Elio.Add(&grafico_Elio);

//---MULTIGRAPH CONFRONTO 2
	TMultiGraph multi_Argon2, multi_Sodio2, multi_Mercurio2;
	//--Confronto Argon
	multi_Argon2.Add(&grafico_sperimentale2);
	multi_Argon2.Add(&grafico_Argon);

	//--Confronto Sodio
	multi_Sodio2.Add(&grafico_sperimentale2);
	multi_Sodio2.Add(&grafico_Sodio);

	//--Confronto Mercurio
	multi_Mercurio2.Add(&grafico_sperimentale2);
	multi_Mercurio2.Add(&grafico_Mercurio);

	TCanvas c2;
	multi_Argon.Draw("AP");
	multi_Argon.SetTitle("Confronto con lo spettro di Ar ; ordine di misurazione ; lunghezza d'onda");
	c2.BuildLegend();
	c2.Print("confronto tra lambda 1.pdf", "pdf");

	TCanvas c3;
	multi_Sodio.Draw("AP");
	multi_Sodio.SetTitle("Confronto con lo spettro di Na ; ordine di misurazione ; lunghezza d'onda");
	c3.BuildLegend();
	c3.Print("confronto tra lambda 2.pdf", "pdf");

	//TApplication app("app", &argc , argv);
	TCanvas c4;
	multi_Mercurio.Draw("AP");
	multi_Mercurio.SetTitle("Confronto con lo spettro di Hg ; ordine di misurazione ; lunghezza d'onda #lambda");
	c4.BuildLegend();
	//app.Run();
	c4.Print("confronto tra lambda 3.pdf", "pdf");

	TCanvas c8;
	multi_Elio.Draw("AP");
	multi_Elio.SetTitle("Confronto con lo spettro di He ; ordine di misurazione ; lunghezza d'onda");
	c8.BuildLegend();
	c8.Print("confronto tra lambda 4.pdf", "pdf");

	TCanvas c5;
	multi_Argon2.Draw("AP");
	multi_Argon2.SetTitle("Confronto con lo spettro di Ar ; ordine di misurazione ; lunghezza d'onda");
	c5.BuildLegend();
	c5.Print("confronto2 tra lambda 1.pdf", "pdf");

	TCanvas c6;
	multi_Sodio2.Draw("AP");
	multi_Sodio2.SetTitle("Confronto con lo spettro di Na ; ordine di misurazione ; lunghezza d'onda");
	c6.BuildLegend();
	c6.Print("confronto2 tra lambda 2.pdf", "pdf");

	//TApplication app("app", &argc , argv);
	TCanvas c7;
	multi_Mercurio2.Draw("AP");
	multi_Mercurio2.SetTitle("Confronto con lo spettro di Hg ; ordine di misurazione ; lunghezza d'onda #lambda");
	c7.BuildLegend();
	//app.Run();
	c7.Print("confronto2 tra lambda 3.pdf", "pdf");


  return 0;
}
