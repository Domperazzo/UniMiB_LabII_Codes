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
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TVirtualPad.h"


int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati1;
  dati1.open("RC100_ch1.txt", std::ios::in);
  std::vector<double> v_secondi1, v_corrente1;
  while (true) {
      double secondi1, corrente1;
	    dati1 >> secondi1 >> corrente1;
      if(dati1.eof()==true) break;
      v_secondi1.push_back(secondi1);
      v_corrente1.push_back(corrente1);
  }
	dati1.close();

  std::ifstream dati2;
  dati2.open("RC100_ch2.txt", std::ios::in);
  std::vector<double> v_secondi2, v_corrente2;
  while (true) {
      double secondi2, corrente2;
      dati2 >> secondi2 >> corrente2;
      if(dati2.eof()==true) break;
      v_secondi2.push_back(secondi2);
      v_corrente2.push_back(corrente2);
  }
  dati2.close();

  TGraph grafico1, grafico2;
  for (int i = 0; i < v_secondi1.size(); i++) {
    grafico1.SetPoint(i, v_secondi1.at(i), v_corrente1.at(i));
    grafico2.SetPoint(i, v_secondi2.at(i), v_corrente2.at(i));
  }


  TCanvas c1;
  grafico1.Draw("ALP");
  theApp.Run();


  return 0;
}
