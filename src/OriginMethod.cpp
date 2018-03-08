#include <RooRealVar.h>
#include <TApplication.h>
#include <TCanvas.h>
#include "OriginMethod.h"
#include "RooAddPdf.h"
#include "RooVoigtian.h"
#include "RooMyPdf.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooMinimizer.h"
#include "argparse.h"

using namespace RooFit;
using namespace std;

double calculate_sqare(vector<double> l, vector<double> r)
{
    double result = 0;
    for (int i = 0; i < l.size(); i++) {
        result += (r[i] - l[i]) * (r[i] - l[i]);
    }
    return sqrt(result);
}

int main(int argc, char **argv)
{
    //ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    RooFit::PrintLevel(5);
    ArgumentParser parser;
    parser.addArgument("-b", "--bin", 1, false);
    parser.addArgument("-d", "--debug", 0, true);
    parser.addArgument("-o", "--output", 1, true);
    parser.parse(argc, argv);
    DebugTimeInfo *debug;
    if (parser.exists("debug")) {
        debug = new DebugTimeInfo(parser.retrieve<std::string>("output"));
        debug->AddKeys("type");
        debug->AddKeys("total");
        debug->AddKeys("s1");
        debug->AddKeys("s2");
        debug->AddKeys("s3");
        debug->AddKeys("s4");
        debug->AddKeys("s5");
        debug->AddKeys("s6");
        debug->AddKeys("s7");
        debug->AddKeys("s8");
    } else {
        debug = nullptr;
    }
    int bins = atoi(parser.retrieve<std::string>("bin").c_str());
//    TApplication *myapp = new TApplication("App", &argc, argv);
    RooRealVar x("x", "x", 15, 0, 30);
    RooRealVar width("width", "width", 10, 8, 12);
    RooRealVar mean("mean", "mean", 10, 8, 12);
    RooRealVar sigma("sigma", "sigma", 0.5, 0.5, 0.5);
    RooMyPdf my_pdf_1("my_pdf_1", "MyPDF", x, mean, width, bins, 0, debug);
    RooMyPdf my_pdf_2("my_pdf_2", "MyPDF", x, mean, width, bins, 1, debug);
    RooMyPdf my_pdf_3("my_pdf_3", "MyPDF", x, mean, width, bins, 2, debug);
    RooMyPdf my_pdf_4("my_pdf_4", "MyPDF", x, mean, width, bins, 3, debug);
    RooMyPdf my_pdf_5("my_pdf_4", "MyPDF", x, mean, width, bins, 4, debug);
    my_pdf_3.cuda_gaus_prepare();
    RooVoigtian voigtian_pdf("voigtian_pdf", "VoigtianPdf", x, mean, width, sigma);
//    RooPlot *xfram = x.frame();
    std::vector<double> v1, v2, v3, v4, v5, v6;
    for (int i = 0; i < 10000; i++) {
        x.setVal(x.getMin() + (x.getMax() - x.getMin()) / 12000. * (1000 + i));
        RooArgSet *set = new RooArgSet(x);
        v1.push_back(voigtian_pdf.getValV(set));
        v2.push_back(my_pdf_1.getValV(set));
        v3.push_back(my_pdf_2.getValV(set));
        v4.push_back(my_pdf_3.getValV(set));
        v5.push_back(my_pdf_4.getValV(set));
        v6.push_back(my_pdf_5.getValV(set));
    }
    cout << "gaus diff: " << calculate_sqare(v2, v1) << endl;
    cout << "normal diff: " << calculate_sqare(v3, v1) << endl;
    cout << "gaus cuda diff: " << calculate_sqare(v4, v1) << endl;
    cout << "normal cuda diff: " << calculate_sqare(v5, v1) << endl;
    cout << "normal cuda tuned diff: " << calculate_sqare(v6, v1) << endl;
    if (debug != nullptr) {
        delete (debug);
    }

//    voigtian_pdf.plotOn(xfram, LineColor(1));
//    my_pdf_1.plotOn(xfram, LineColor(2));
//    my_pdf_2.plotOn(xfram, LineColor(3));
//    my_pdf_3.plotOn(xfram, LineColor(4));
//    my_pdf_4.plotOn(xfram, LineColor(5));
//    auto h_voigtian_pdf = voigtian_pdf.createHistogram("x", 1000000);
//    auto h_my_pdf_1 = my_pdf_1.createHistogram("x", 100000);
//    auto h_my_pdf_2 = my_pdf_2.createHistogram("x", 100000);
//    auto h_my_pdf_3 = my_pdf_3.createHistogram("x", 100000);
//
//    std::cout << "Diff h1" << h_voigtian_pdf->Chi2Test(h_my_pdf_1) << std::endl;
//    std::cout << "Diff h2" << h_voigtian_pdf->Chi2Test(h_my_pdf_2) << std::endl;
//    std::cout << "Diff h3" << h_voigtian_pdf->Chi2Test(h_my_pdf_3) << std::endl;

//    auto data = voigtian_pdf.generate(x, 10000);
//    auto h = data->createHistogram("x", 100, 0, 30);
//    h->Draw();
//    my_pdf_3.fitTo(*data);
//    my_pdf_3.plotOn(xfram);

//    TCanvas *c = new TCanvas("voigtian", "voigtian");
//    gPad->SetLeftMargin(0.15);
//    xfram->GetYaxis()->SetTitleOffset(1.4);
//    xfram->SetTitle("");
//    xfram->Draw();

//    TCanvas *c2 = new TCanvas("gaus_evaluate", "gaus_evaluate");
//    c2->cd();
//    debug->tree->Draw("total", "type==0");
//    c2->SetLogy(true);
//    c2->Update();
//
//    TCanvas *c3 = new TCanvas("normal_evaluate", "normal_evaluate");
//    c3->cd();
//    debug->tree->Draw("total", "type==1");
//    c3->SetLogy(true);
//    c3->Update();
//
//    TCanvas *c4 = new TCanvas("cuda_normal_evaluate", "cuda_normal_evaluate");
//    c4->cd();
//    debug->tree->Draw("total", "type==2");
//    c4->SetLogy(true);
//    c4->Update();
//
//    TCanvas *c5 = new TCanvas("cuda_gaus_evaluate", "cuda_gaus_evaluate");
//    c5->cd();
//    debug->tree->Draw("total", "type==3");
//    c5->SetLogy(true);
//    c5->Update();
//    myapp->Run();
}