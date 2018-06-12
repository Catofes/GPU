//
// Created by herbertqiao on 6/12/18.
//

#include <RooRealVar.h>
#include <TApplication.h>
#include <TCanvas.h>
#include "RooMyPdf.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "argparse.h"
#include "TH1F.h"
#include "iostream"

using namespace std;

int main(int argc, char **argv)
{
    RooFit::PrintLevel(RooFit::INFO);
    ArgumentParser parser;
    parser.addArgument("-b", "--bin", 1, false);
    parser.parse(argc, argv);
    TApplication *app = new TApplication("app", &argc, argv);
    int bins = atoi(parser.retrieve<std::string>("bin").c_str());

    RooRealVar x("x", "x", 100, 50, 150);
    RooRealVar width_1("width_1", "width_1", 30, 0, 50);
    RooRealVar width_2("width_2", "width_2", 30, 0, 50);
    RooRealVar mean_1("mean_1", "mean_1", 80, 50, 150);
    RooRealVar mean_2("mean_2", "mean_2", 120, 50, 150);
    RooRealVar ratio("a", "a", 1.0, 0, 10);

    RooMyPdf cpu_pdf("cpu_pdf", "MyPDF", x, mean_1, mean_2, width_1, width_2, ratio, false, bins);

    cout << "Generating Data ..." << endl;
    auto data = cpu_pdf.generate(x, 10000);
    //auto h = data->createHistogram("x", 200, 50, 150);
    //h->Draw();

    cout << "Done. Fitting ..." << endl;

    RooMyPdf gpu_pdf("gpu_pdf", "MyPDF", x, mean_1, mean_2, width_1, width_2, ratio, true, bins);
    RooPlot *xfram = x.frame();
    gpu_pdf.fitTo(*data);

    cout << "Done. Drawing ..." << endl;
    data->plotOn(xfram);
    gpu_pdf.plotOn(xfram);
    xfram->Draw();
    app->Run();
}