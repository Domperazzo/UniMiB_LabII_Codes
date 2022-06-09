// Esercizio 3

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>

#include "TF1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TApplication.h"
#include "TLatex.h"
#include "TStyle.h"

using namespace std ;

double minimo (const vector<double> & v)
  {
    double min = v.at (0) ;
    for (int i = 0 ; i < v.size () ; ++i)
      if (v.at (i) < min) min = v.at (i) ;
    return min ;
  }

double massimo (const vector<double> & v)
  {
    double max = v.at (0) ;
    for (int i = 0 ; i < v.size () ; ++i)
      if (v.at (i) > max) max = v.at (i) ;
    return max ;
  }

double maxval (double a, double b)
  {
    if (a > b) return a ;
    return b ;
  }


int main (int argc, char ** argv){
    // lettura del file di eventi
    TApplication * theApp = new TApplication("theApp", &argc, argv);
    gStyle->SetOptFit(1111);
    ifstream f;
    f.open ("interferometro.txt", std::ios::in);

    vector<double> v_eventi;
    while (true)
      {
        double evento;
        f >> evento;
        if (f.eof () == true) break;
        v_eventi.push_back (evento);
      }
    f.close () ;

    cout << "letti " << v_eventi.size () << " eventi" << endl;
    cout << "minimo degli eventi: " << minimo (v_eventi) << endl;
    cout << "massimo degli eventi: " << massimo (v_eventi) << endl;

    // preparazione e riempimento dell'istogramma

    double min = floor (minimo (v_eventi));
    double max = ceil (massimo (v_eventi));
//    int N_bin  = round (sqrt (v_eventi.size ()) / 2.);
    int N_bin  = round (sqrt (v_eventi.size ()));
    N_bin =8; //maxval (N_bin, 10);

    TH1F * h_eventi = new TH1F("h_eventi", "", N_bin, min, max);
    for (int i = 0 ; i < v_eventi.size () ; ++i) h_eventi->Fill (v_eventi.at (i));


    TF1 * model = new TF1 ("model", "gaus(0)", 0., 20.);
    model->SetLineColor (kBlue + 2);
    model->SetLineWidth (4);
    model->SetLineStyle (1);

    //double p0 = v_eventi.size ();    // eventi
    double p1 = 0.5 * (max - min);   // media
    double p2 = h_eventi->GetRMS ();  // sigma

    //model->SetParameter (0, p0);
    model->SetParameter (1, p1);
    model->SetParameter (2, p2);



    h_eventi->Fit ("model");



    TCanvas * c1 = new TCanvas("c1", "", 800, 800);
    c1->SetLeftMargin (0.15);
    c1->cd();
    h_eventi->GetXaxis ()->SetTitle ("#Deltad (#mum)");
    h_eventi->GetYaxis ()->SetTitle ("Frequenza eventi");
    h_eventi->SetFillColor (kTeal-8);
    h_eventi->SetLineColor (kGray);
    h_eventi->SetTitle("Calibrazione");
    h_eventi->GetFunction ("model")->SetLineColor (kRed);
    h_eventi->Draw ();
    c1->Modified();
    c1->Update();
    c1->SaveAs("main_04.pdf") ;

    theApp->Run();

    TFitResultPtr fit_result_MQ = h_eventi->Fit ("model", "SQ+");
    cout << "Minimi quadrati" << '\n';
    cout << endl;
    cout.precision (3);
    //cout << "integrale                 : " << model->GetParameter (0) << "\t+- " << model->GetParError (0) << endl;
    cout << "media                     : " << model->GetParameter (1) << "\t+- " << model->GetParError (1) << endl;
    cout << "sigma                     : " << model->GetParameter (2) << "\t+- " << model->GetParError (2) << endl;

    TFitResultPtr fit_result_ML = h_eventi->Fit ("model", "SLQ+");
    cout << "Massima verosimiglianza" << '\n';
    cout << endl;
    cout << "probabilità associata a ML: " << model->GetProb () << endl;
    //cout << "integrale                 : " << model->GetParameter (0) << "\t+- " << model->GetParError (0) << endl;
    cout << "media                     : " << model->GetParameter (1) << "\t+- " << model->GetParError (1) << endl;
    cout << "sigma                     : " << model->GetParameter (2) << "\t+- " << model->GetParError (2) << endl;
    return 0 ;
  }
