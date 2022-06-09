/*
c++ -o browster browster.cpp `root-config --cflags --glibs`
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
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TMatrixD.h"

using namespace std;

bool readData(string fileName, vector<double> x_Vec, vector<double> y_Vec, vector<double> x_err, vector<double> y_err){
  std::fstream myFile;

  myFile.open(fileName.c_str(),std::ios::in);
  if (myFile.good() == false)
    {
      return false;
    }

  double angolo,intensita, err_ang, err_int;
  while (true)
    {
      myFile >> angolo >> intensita >> err_ang >> err_int;
      if (myFile.eof() == true) break;
      x_Vec.push_back(angolo);
      y_Vec.push_back(intensita);
      x_err.push_back(err_ang);
      y_err.push_back(err_int);
    }
  myFile.close();

  return true;
}

int main(int argc, char** argv)
{
  vector<double> ang_vec, int_vec, err_ang, err_int;
  //readData("broow.txt",ang_vec, int_vec, err_ang, err_int);
  gStyle->SetOptFit(1112);

  TApplication* myApp = new TApplication("myApp", NULL, NULL);
  TCanvas* myC = new TCanvas("myC","myC",0,0,700,500);
  //TGraphErrors *myGraph = new TGraphErrors (ang_vec.size(), &int_vec[0], &err_ang[0], &err_int[0]);
  TGraphErrors *myGraph = new TGraphErrors (argv[1]);
  myGraph->SetMarkerStyle(20);
  myGraph->SetTitle("Intensita' in funzione dell'angolo");
  myGraph->GetXaxis()->SetTitle("#theta [°]");
  myGraph->GetYaxis()->SetTitle("I [mA]");

  /*for (int i; i<5; i++) {
    myGraph->SetPointError(i, 0, readData("riflessione_err.txt", v_err_vec));
  }
*/
  TF1* myFun = new TF1 ("myFun","[0]*x*x+[1]*x+[2]");
  myFun->SetParName(0,"A");
  myFun->SetParName(1,"B");
  myFun->SetParName(2,"C");


  myC->cd();
  myGraph->Draw("AP");
  myGraph->Fit("myFun", "S");
  myC->Modified();
  myC->Update();

  TFitResultPtr result=myGraph->Fit("myFun", "S");
  TMatrixD cov = result->GetCovarianceMatrix();
  result->PrintCovMatrix(cout);
  cout << "\nIl risultato del fit lineare è:" << endl;
  cout << "A = " << myFun->GetParameter(0) << " +/- " << myFun->GetParError(0) << endl;
  cout << "B = " << myFun->GetParameter(1) << " +/- " << myFun->GetParError(1) << endl;
  cout << "C = " << myFun->GetParameter(2) << " +/- " << myFun->GetParError(2) << endl;


  myApp->Run();
  return 0;
}
