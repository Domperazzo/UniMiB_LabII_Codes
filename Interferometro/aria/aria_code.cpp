/*
c++ -o template template.cpp `root-config --cflags --glibs`
*/

#include <iostream>

#include <TStyle.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include "TLatex.h"

using namespace std;

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      cout << "Not enough parameters: ./esercizio01 filename.txt" << endl;
      return 1;
    }

  gStyle->SetOptFit(1112);

  TApplication* myApp = new TApplication("myApp", NULL, NULL);
  TCanvas* myC = new TCanvas("myC","myC",0,0,700,500);
  TGraphErrors* myGraph = new TGraphErrors(argv[1]);
  myGraph->SetMarkerStyle(20);
  myGraph->SetTitle("Indice rifrazione aria");
  myGraph->GetXaxis()->SetTitle("#DeltaN");
  myGraph->GetYaxis()->SetTitle("#DeltaP [kPa]");
  for (int i; i<5; i++) {
    myGraph->SetPointError(i, 0, 2);
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
