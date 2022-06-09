/*
c++ -o polarizzazione_coseni polarizzazione_lin.cpp `root-config --cflags --glibs`
*/

#include <iostream>

#include <TStyle.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include "TLatex.h"
#include <fstream>

using namespace std;

bool readData(string fileName, vector<double> first_vec, vector<double> second_vec){
  std::fstream myFile;

  myFile.open(fileName.c_str(),std::ios::in);
  if (myFile.good() == false)
    {
      return false;
    }

  double first, second;
  while (true)
    {
      myFile >> first >> second;
      if (myFile.eof() == true) break;
      first_vec.push_back(first);
      second_vec.push_back(second);
    }
  myFile.close();

  return true;
}

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      cout << "Not enough parameters: ./esercizio01 filename.txt" << endl;
      return 1;
    }

  vector<double> v_err_vec;

  gStyle->SetOptFit(1112);

  TApplication* myApp = new TApplication("myApp", NULL, NULL);
  TCanvas* myC = new TCanvas("myC","myC",0,0,700,500);
  TGraphErrors* myGraph = new TGraphErrors(argv[1]);
  myGraph->SetMarkerStyle(20);
  myGraph->SetTitle("Polarizzazione");
  myGraph->GetXaxis()->SetTitle("cos#theta");
  myGraph->GetYaxis()->SetTitle("Intensita' segnale [mA]");
  for (int i; i<5; i++) {
    myGraph->SetPointError(i, 0, .02);
  }

  TF1* myFun = new TF1 ("myFun","[0]*x+[1]");
  myFun->SetParName(0,"A");
  myFun->SetParName(1,"B");
  myFun->SetParameter(0,8);
  myFun->SetParameter(1,0.004);

  myC->cd();
  myGraph->Draw("AP");
  myGraph->Fit("myFun");
  myC->Modified();
  myC->Update();

  cout << "\nIl risultato del fit lineare è:" << endl;
  cout << "Coefficiente angolare = " << myFun->GetParameter(0) << " +/- " << myFun->GetParError(0) << endl;
  cout << "Intercetta = " << myFun->GetParameter(1) << " +/- " << myFun->GetParError(1) << endl;

  myApp->Run();
  return 0;
}
