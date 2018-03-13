import ROOT

data = '''
1000	2000	5000	10000	20000	50000	100000	200000	500000	1000000	2000000	5000000	10000000	20000000	50000000	100000000	200000000
201978.222	390286.6296	1002573.632	1939073.078	3861769.681	9642033.291	19337215.41	38571405.16	96346875.83	192520007.1							
136145.7634	271690.1999	679476.3514	1350056.82	2692366.42	6707720.963	13536351.74	26822811.38	67501444.75	135325862.6							
675838.3719	689284.174	682012.0855	692447.0363	708162.8283	707609.1143	704604.822	715761.3075	794323.4858	1235948.57	1769308.889	3952166.474	8150731.445	14985905.01	37768743.25	80963736.11	151628833.5
665486.6821	677809.0957	669579.8981	676525.0607	689790.3648	689647.3321	686758.718	692329.744	718036.9363	984642.0624	1381392.069	2981092.759	6207988.377	11076530.44	27949132.55	61057992.85	111894993.5
'''

lines = data.strip().split("\n")
lines = [line.strip() for line in lines]
lines = [line.split() for line in lines]
X = lines[0]
X = [int(v) for v in X]
Gauss = lines[1]
Gauss = [float(v) for v in Gauss]
Riemann = lines[2]
Riemann = [float(v) for v in Riemann]
GPUGauss = lines[3]
GPUGauss = [float(v) for v in GPUGauss]
GPURiemann = lines[4]
GPURiemann = [float(v) for v in GPURiemann]

tCanvas1 = ROOT.TCanvas("c1")
tCanvas1.cd()
tTmpHist = ROOT.TH2F("tmp1", "", 100, 500, 300000000, 100, 100000, 200000000)
tTmpHist.Draw()
tTmpHist.SetStats(False)
tTmpHist.GetXaxis().SetTitle("Bins")
tTmpHist.GetXaxis().CenterTitle()
tTmpHist.GetYaxis().SetTitle("Time / ns")
tTmpHist.GetYaxis().CenterTitle()

graphGauss = ROOT.TGraph(len(Gauss))
for i in range(0, len(Gauss)):
    graphGauss.SetPoint(i, X[i], Gauss[i])
graphGauss.SetTitle("Gauss")
graphGauss.SetLineColor(28)
graphGauss.SetLineWidth(3)
graphGauss.SetMarkerStyle(20)
graphGauss.SetMarkerColor(28)
graphGauss.SetMarkerSize(1)
graphGauss.Draw("PL")

graphRiemann = ROOT.TGraph(len(Riemann))
for i in range(0, len(Riemann)):
    graphRiemann.SetPoint(i, X[i], Riemann[i])
graphRiemann.SetTitle("Riemann")
graphRiemann.SetLineColor(46)
graphRiemann.SetLineWidth(3)
graphRiemann.SetMarkerStyle(21)
graphRiemann.SetMarkerColor(46)
graphRiemann.SetMarkerSize(1)
graphRiemann.Draw("PL")

graphGPUGauss = ROOT.TGraph(len(GPUGauss))
for i in range(0, len(GPUGauss)):
    graphGPUGauss.SetPoint(i, X[i], GPUGauss[i])
graphGPUGauss.SetTitle("GPU Gauss")
graphGPUGauss.SetLineColor(9)
graphGPUGauss.SetLineWidth(3)
graphGPUGauss.SetMarkerStyle(24)
graphGPUGauss.SetMarkerColor(9)
graphGPUGauss.SetMarkerSize(1)
graphGPUGauss.Draw("PL")

graphGPURiemann = ROOT.TGraph(len(GPURiemann))
for i in range(0, len(GPURiemann)):
    graphGPURiemann.SetPoint(i, X[i], GPURiemann[i])
graphGPURiemann.SetTitle("GPU Riemann")
graphGPURiemann.SetLineColor(8)
graphGPURiemann.SetLineWidth(3)
graphGPURiemann.SetMarkerStyle(25)
graphGPURiemann.SetMarkerColor(8)
graphGPURiemann.SetMarkerSize(1)
graphGPURiemann.Draw("PL")

tLegend = ROOT.TLegend(0.6, 0.15, 0.85, 0.35)
# tLegend.SetTextFont(72)
# tLegend.SetTextSize(0.05)
tLegend.SetFillColor(0)
tLegend.AddEntry(graphGauss, "Gauss", "PL")
tLegend.AddEntry(graphRiemann, "Riemann", "PL")
tLegend.AddEntry(graphGPUGauss, "GPU Gauss", "PL")
tLegend.AddEntry(graphGPURiemann, "GPU Riemann", "PL")
tLegend.Draw()

tCanvas1.SetLogy()
tCanvas1.SetLogx()
tCanvas1.SetTitle("")
input()