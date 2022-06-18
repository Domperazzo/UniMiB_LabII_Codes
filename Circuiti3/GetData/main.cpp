/*
	c++ -o fasi main.cpp `root-config --glibs --cflags`
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

  std::vector<std::string> v_channel1, v_channel2, v_channelMath;
  std::ifstream OpenFile;
  OpenFile.open("filenames.txt", std::ios::in);
  while (true) {
    std::string "ch1", "ch2","math";
    OpenFile << "ch1" << "ch2" << "math";
    if (OpenFile.eof()==true)break;
    v_channel1.push_back("ch1");
    v_channel2.push_back("ch2");
    v_channelMath.push_back("math");
  }
  OpenFile.close();


for (size_t i = 0; i < v_channel1.size(); i++) {

// Apertura CH1
std::ifstream dati1;
dati1.open("v_channel1.at(i)", std::ios::in);
std::vector<double> v_secondi1, v_corrente1;
while (true) {
    double secondi1, corrente1;
    dati1 >> secondi1 >> corrente1;
    if(dati1.eof()==true) break;
    v_secondi1.push_back(secondi1);
    v_corrente1.push_back(corrente1);
}
dati1.close();

// Apertura CH2
std::ifstream dati2;
dati2.open("v_channel2.at(i)", std::ios::in);
std::vector<double> v_secondi2, v_corrente2;
while (true) {
    double secondi2, corrente2;
    dati2 >> secondi2 >> corrente2;
    if(dati2.eof()==true) break;
    v_secondi2.push_back(secondi2);
    v_corrente2.push_back(corrente2);
}
dati2.close();

// Apertura MATH
std::ifstream dati_math;
dati_math.open("v_channelMath.at(i)", std::ios::in);
std::vector<double> v_secondi_math, v_corrente_math;
while (true) {
    double secondi_math, corrente_math;
    dati_math >> secondi_math >> corrente_math;
    if(dati_math.eof()==true) break;
    v_secondi_math.push_back(secondi_math);
    v_corrente_math.push_back(corrente_math);
}
dati_math.close();



TGraphErrors grafico_ch1, grafico_ch2, grafico_mat;

for (int i = 0; i < v_secondi1.size(); i++) {
  grafico_ch1.SetPoint(i, v_secondi1.at(i), v_corrente1.at(i));
  grafico_ch1.SetPointError(i, 0., 0.040);
}

TF1 modello("modello", "[0]*sin([1]*x+[2])");
modello.SetParName(0, "A");
modello.SetParName(1, "#omega");
modello.SetParName(2, "#phi");


TFitResultPtr fit_result_ch1 = grafico_ch1.Fit(&modello, "S");

std::vector<double> v_phiCH1, v_phiCH2, v_phiMATH;

v_phiCH1.push_back(modello.GetParameter(2));

//double phi_ch1 = modello.GetParameter(2);


for (int i = 0; i < v_secondi2.size(); i++) {
  grafico_ch2.SetPoint(i, v_secondi2.at(i), v_corrente2.at(i));
  grafico_ch2.SetPointError(i, 0., 0.040);
}

TFitResultPtr fit_result_ch2 = grafico_ch2.Fit(&modello, "S");

v_phiCH2.push_back(modello.GetParameter(2));
//double phi_ch2 = modello.GetParameter(2);


for (int i = 0; i < v_secondi_math.size(); i++) {
  grafico_mat.SetPoint(i, v_secondi_math.at(i), v_corrente_math.at(i));
  grafico_mat.SetPointError(i, 0., 0.040);
}

TFitResultPtr fit_result_math = grafico_mat.Fit(&modello, "S");

v_phiMATH.push_back(modello.GetParameter(2));
//double phi_math = modello.GetParameter(2);
/*
std::cout << "phi 1\t" << phi_ch1 << '\n';
std::cout << "phi 2\t" << phi_ch2 << '\n';
std::cout << "phi math\t" << phi_math << '\n';
*/


std::ofstream SaveFile("channels_phase.txt");
for (size_t i = 0; i < v_phiCH1.size(); i++) {
  SaveFile << v_phiCH1.at(i) << v_phiCH2.at(i) << v_phiMATH.at(i);
}
SaveFile.close();
    }
  return 0;
}
