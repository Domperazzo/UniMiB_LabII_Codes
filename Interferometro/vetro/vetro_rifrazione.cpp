/*
  c++ -o vetro vetro.cpp `root-config --cflags --glibs`
*/

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TApplication.h"

using namespace std;

bool readData(std::string fileName, std::vector<double>& valVec){
  std::fstream myFile;

  myFile.open(fileName.c_str(),std::ios::in);
  if (myFile.good() == false)
    {
      return false;
    }

  double val;
  while (true)
    {
      myFile >> val;
      if (myFile.eof() == true) break;
      valVec.push_back(val);
    }
  myFile.close();

  return true;
}

int main(int argc, char const *argv[]) {

  TApplication theApp("myApp", NULL, NULL);

  vector<double> v_indice;
  readData("vetro_indice.txt", v_indice);


  TH1F indice_rifrazione("Indice di rifrazione del vetro", "Indice di rifrazione del vetro", 100, 1.3, 1.7);
  for (size_t i = 0; i < v_indice.size(); i++) {
    indice_rifrazione.Fill(v_indice.at(i));
  }

  indice_rifrazione.GetXaxis ()->SetTitle ("Indice rifrazione vetro");
  indice_rifrazione.GetYaxis ()->SetTitle ("Numero conteggi");

  TCanvas c1("c1", "c1", 650, 650);
  indice_rifrazione.Fit("gaus");
  indice_rifrazione.Draw();
  c1.Print("istogramma.pdf", "pdf");
  theApp.Run();
  return 0;
}
