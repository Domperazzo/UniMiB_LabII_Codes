/*
	c++ -o trova_zeri trova_zeri.cpp `root-config --glibs --cflags`
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

void trova_zeri(std::vector<double> v_dati, std::vector<int> v_posizione_zeri){
  int i;
  for (i = 0; i < v_dati.size(); i++) {
    if (v_dati.at(i)==0.) {
      v_posizione_zeri.push_back(i);
    }
  }
}

int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

// Apertura CH1
std::ifstream dati1;
dati1.open("RC_100hz_CH1.txt", std::ios::in);
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
dati2.open("RC_100hz_CH2.txt", std::ios::in);
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
dati_math.open("RC_100hz_MATH.txt", std::ios::in);
std::vector<double> v_secondi_math, v_corrente_math;
while (true) {
    double secondi_math, corrente_math;
    dati_math >> secondi_math >> corrente_math;
    if(dati_math.eof()==true) break;
    v_secondi_math.push_back(secondi_math);
    v_corrente_math.push_back(corrente_math);
}
dati_math.close();

std::vector<int> v_posZeri_CH1, v_posZeri_CH2, v_posZeri_MATH;


for (int i = 0; i < v_corrente1.size(); i++) {
  if (v_corrente1.at(i)==0.) {
    v_posZeri_CH1.push_back(i);
  }
}
for (int i = 0; i < v_corrente2.size(); i++) {
  if (v_corrente2.at(i)==0.) {
    v_posZeri_CH2.push_back(i);
  }
}
for (int i = 0; i < v_corrente_math.size(); i++) {
  if (v_corrente_math.at(i)==0.) {
    v_posZeri_MATH.push_back(i);
  }
}

std::vector<double> v_secondi_zeri1, v_secondi_zeri2, v_secondi_zeri_math;

for (int i = 0; i < v_posZeri_CH1.size(); i++) {
  v_secondi_zeri1.push_back(v_secondi1.at(v_posZeri_CH1.at(i)));
}

for (int i = 0; i < v_posZeri_CH2.size(); i++) {
  v_secondi_zeri2.push_back(v_secondi2.at(v_posZeri_CH2.at(i)));
}

for (int i = 0; i < v_posZeri_MATH.size(); i++) {
  v_secondi_zeri_math.push_back(v_secondi_math.at(v_posZeri_MATH.at(i)));
}

std::cout << "CH1" << '\n';
for (int i = 0; i < v_secondi_zeri1.size(); i++) {
  if (v_secondi_zeri1.at(i) > 0.) {
    std::cout << v_secondi_zeri1.at(i) << '\n';
  }
}

std::cout << "CH2" << '\n';
for (int i = 0; i < v_secondi_zeri2.size(); i++) {
  if (v_secondi_zeri2.at(i) > 0.) {
    std::cout << v_secondi_zeri2.at(i) << '\n';
  }
}

std::cout << "MATH" << '\n';
for (int i = 0; i < v_secondi_zeri_math.size(); i++) {
  if (v_secondi_zeri_math.at(i) > 0.) {
    std::cout << v_secondi_zeri_math.at(i) << '\n';
  }

}

/*
std::ofstream myfile;
myfile.open ("data_file.txt", std::ios::app);
myfile << media_zeri1 << media_zeri2 << media_zeriMath;
myfile.close();
*/


return 0;
}
