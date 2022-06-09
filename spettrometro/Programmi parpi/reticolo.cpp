/*
c++ -o reticolo reticolo.cpp `root-config --glibs --cflags`
*/

#include<stdlib.h>
#include<cmath>
#include<iostream>
#include<fstream>
#include<vector>
#include "TGraph.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TGraphErrors.h"

using namespace std;

double retta (double* x , double* p){

	return p[0]*x[0];
}

int main ( int argc , char** argv ) {
	gStyle->SetOptFit(1112);
	ifstream dati;
	dati.open("angoli_massimi.txt", ios::in);

	double theta0 , val, err_val;
	dati >> theta0;
	dati >> val;

	theta0 = (theta0 + val/60)*M_PI/180 ;

	double theta;
	TGraphErrors grafico;

	for ( int i = 0 ; i < 3 ; i++ ) {				/* questo ciclo è fatto per legge i dati. ad ogni giro */
									/* legge un angolo in meno.  		                */
		dati >> theta;
		dati >> val;
		theta = (( theta + val/60 ) * M_PI/180) - theta0 ;

		grafico.SetPoint(i, i+1, sin(theta));
		grafico.SetPointError(i, 0., (5*M_PI/3)/300);
	}

	dati.close();




	TF1 modello ("modello", retta , 0 , 3, 1);
	TFitResultPtr fit_result = grafico.Fit(&modello, "S");

	TApplication app ("app", &argc , argv);
	TCanvas c1;
	grafico.SetMarkerStyle(20);
	grafico.SetMarkerSize(0.5);
	grafico.SetTitle("Legge diffrazione ; ordine del massimo ; sin(#theta - #alpha)");
	grafico.Draw("AP");
	app.Run();

}
