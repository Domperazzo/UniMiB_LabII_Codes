/*
	c++ -o partitore partitore.cpp `root-config --glibs --cflags`
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


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
	//dati.open("leggeOhm.txt", std::ios::in);
  dati.open("partitore_resistivo.txt", std::ios::in);
  std::vector<double> v_RLoad, v_Vout;
  while (true) {
      double volt, resistenza;
	    dati >> resistenza >> volt;
      if(dati.eof()==true) break;
      v_Vout.push_back(volt);
      v_RLoad.push_back(resistenza);
  }
	dati.close();


  TGraphErrors grafico;
  for (int i = 0; i < v_RLoad.size(); i++) {
    grafico.SetPoint(i+1, v_RLoad.at(i), v_Vout.at(i));
    //grafico.SetPointError(i+1, 0, 0.015);
  }

  TF1 modello ("modello", "x/(2*x+250000)");
  TMultiGraph multi;
  multi.Add(&grafico);
  multi.Add(&modello);

  TCanvas c1;
  multi.SetTitle("Partitore resistivo; V out; R load");
  multi.GetHistogram()-> GetXaxis()->SetRangeUser(-100, 13000000);
  grafico.SetMarkerStyle(20);
  grafico.SetMarkerSize(0.5);
  //grafico.Draw("AP");
  c1.Print();
  theApp.Run();





  return 0;
}
