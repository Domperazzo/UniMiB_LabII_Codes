/*
	c++ -o diodoSoglia diodoSoglia.cpp `root-config --glibs --cflags`
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
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TVirtualPad.h"

int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("diodo_invertito.txt", std::ios::in);
  std::vector<double> v_Volt, v_Corrente;
  while (true) {
      double Volt, Corrente;
	    dati >> Volt >> Corrente;
      if(dati.eof()==true) break;
      v_Volt.push_back(Volt);
      v_Corrente.push_back(0.001*Corrente);
  }
	dati.close();

  std::ifstream dati2;
  dati2.open("diodo.txt", std::ios::in);
  std::vector<double> v_volt, v_corrente;
  while (true) {
      double volt, corrente;
      dati2 >> volt >> corrente;
      if(dati2.eof()==true) break;
      v_volt.push_back(volt);
      v_corrente.push_back(0.001*corrente);
  }
  dati2.close();

  std::ifstream err_dati;
  err_dati.open("err_diodo.txt", std::ios::in);
  std::vector<double> v_err;
  while (true) {
      double err;
      err_dati >> err;
      if(err_dati.eof()==true) break;
      v_err.push_back(err);
  }
  err_dati.close();



  TGraphErrors diodo_reverse;
  for (int i = 0; i < 9; i++) {
    diodo_reverse.SetPoint(i, v_Volt.at(i), v_Corrente.at(i));
    diodo_reverse.SetPointError(i, 0., 0.015);
  }
  diodo_reverse.SetTitle("Legge di Shockley");
  diodo_reverse.GetHistogram()-> GetXaxis()->SetTitle("d.d.p. (V)");
  diodo_reverse.GetHistogram()-> GetYaxis()->SetTitle("corrente (A)");

  TF1 modello("modello", "[0]*x+[1]");
  TFitResultPtr fit_result = diodo_reverse.Fit(&modello, "S");

  diodo_reverse.SetMarkerStyle(20);
  diodo_reverse.SetMarkerSize(0.5);

  TGraphErrors diodo;
  for (int i = 0; i < v_volt.size()-9; i++) {
    diodo.SetPoint(i, v_volt.at(i), v_corrente.at(i));
    diodo.SetPointError(i, 0., v_err.at(i));
  }
  diodo.SetTitle("Legge di Shockley");
  diodo.GetHistogram()-> GetXaxis()->SetTitle("d.d.p. (V)");
  diodo.GetHistogram()-> GetYaxis()->SetTitle("corrente (A)");


  diodo.SetMarkerStyle(20);
  diodo.SetMarkerSize(0.5);

  TMultiGraph multi_grafico;
  multi_grafico.Add(&diodo_reverse);
  multi_grafico.Add(&diodo);

	TCanvas c2;
  c2.SetGrid(1,1);
  multi_grafico.Draw("AP");
	multi_grafico.SetTitle("Legge di Shockley ; Tensione [V] ; Corrente [A]");
	theApp.Run();
	c2.Print("diodo.pdf", "pdf");

  return 0;
}
