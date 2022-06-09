/*
c++ -o riflessione riflessione.cpp `root-config --cflags --glibs`
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

bool readData(string fileName, vector<double> x_Vec){
  std::fstream myFile;

  myFile.open(fileName.c_str(),std::ios::in);
  if (myFile.good() == false)
    {
      return false;
    }

  double x;
  while (true)
    {
      myFile >> x;
      if (myFile.eof() == true) break;
      x_Vec.push_back(x);
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
  myGraph->SetTitle("Indice rifrazione aria");
  myGraph->GetXaxis()->SetTitle("#DeltaN");
  myGraph->GetYaxis()->SetTitle("#DeltaP [kPa]");
  for (int i; i<5; i++) {
    myGraph->SetPointError(i, 0, readData("riflessione_err.txt", v_err_vec));
  }

  TF1* myFun = new TF1 ("myFun","[0]*x+[1]");
  myFun->SetParName(0,"m");
  myFun->SetParName(1,"q");
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
