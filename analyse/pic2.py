import ROOT

data = '''
1000	2000	5000	10000	20000	50000	100000	200000	500000	1000000	2000000	5000000	10000000	20000000	50000000	100000000	200000000
224188.7004	445534.3216	1116443.876	2230918.153	4476439.891	11156217.26	22043587.97	44258584.77	110262770.1	221767305.5							
154501.4818	315668.209	768402.503	1535415.037	3051301.315	7645049.233	15451834.91	30589531.28	76414678.06	152769897.6							
668315.0636	684573.7449	672426.6429	673484.3031	679746.8141	681683.8548	686961.1488	700179.2421	705874.9658	1090811.015	1856043.604	4197993.894	8640079.153	16006945.3	40389069.56	89928003.75	162458074.2
668709.1032	684309.1852	671952.4095	674039.095	679906.453	678596.7572	686233.9364	695729.571	693618.9174	906053.4094	1480131.933	3240281.947	6709316.591	12113703.42	30591342.2	70037103.46	122803131.6
195285.6236	202370.9221	197224.1263	196829.6178	205277.2303	202259.6146	210137.4721	290780.7822	411213.7445	702185.9597	1273181.293	3030897.153	6503725.878	11904652.07	30383175.64	69836575.46	122588802.1
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
GPURiemannTuned = lines[5]
GPURiemannTuned = [float(v) for v in GPURiemannTuned]


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
tLegend.AddEntry(graphGauss, "CPU Gauss", "PL")
tLegend.AddEntry(graphRiemann, "CPU Riemann", "PL")
tLegend.AddEntry(graphGPUGauss, "GPU Gauss", "PL")
tLegend.AddEntry(graphGPURiemann, "GPU Riemann", "PL")
tLegend.Draw()

tCanvas1.SetLogy()
tCanvas1.SetLogx()
tCanvas1.SetTitle("")


tCanvas2 = ROOT.TCanvas("c2")
tCanvas2.cd()
tTmpHist2 = ROOT.TH2F("tmp2", "", 100, 500, 300000000, 100, 100000, 200000000)
tTmpHist2.Draw()
tTmpHist2.SetStats(False)
tTmpHist2.GetXaxis().SetTitle("Bins")
tTmpHist2.GetXaxis().CenterTitle()
tTmpHist2.GetYaxis().SetTitle("Time / ns")
tTmpHist2.GetYaxis().CenterTitle()

graphRiemann2 = ROOT.TGraph(len(Riemann))
for i in range(0, len(Riemann)):
    graphRiemann2.SetPoint(i, X[i], Riemann[i])
graphRiemann2.SetTitle("Riemann")
graphRiemann2.SetLineColor(46)
graphRiemann2.SetLineWidth(3)
graphRiemann2.SetMarkerStyle(21)
graphRiemann2.SetMarkerColor(46)
graphRiemann2.SetMarkerSize(1)
graphRiemann2.Draw("PL")

graphGPURiemann2 = ROOT.TGraph(len(GPURiemann))
for i in range(0, len(GPURiemann)):
    graphGPURiemann2.SetPoint(i, X[i], GPURiemann[i])
graphGPURiemann2.SetTitle("GPU Riemann")
graphGPURiemann2.SetLineColor(8)
graphGPURiemann2.SetLineWidth(3)
graphGPURiemann2.SetMarkerStyle(25)
graphGPURiemann2.SetMarkerColor(8)
graphGPURiemann2.SetMarkerSize(1)
graphGPURiemann2.Draw("PL")

graphGPURiemannTuned = ROOT.TGraph(len(GPURiemannTuned))
for i in range(0, len(GPURiemannTuned)):
    graphGPURiemannTuned.SetPoint(i, X[i], GPURiemannTuned[i])
graphGPURiemannTuned.SetTitle("GPU Riemann Tuned")
graphGPURiemannTuned.SetLineColor(49)
graphGPURiemannTuned.SetLineWidth(3)
graphGPURiemannTuned.SetMarkerStyle(20)
graphGPURiemannTuned.SetMarkerColor(49)
graphGPURiemannTuned.SetMarkerSize(1)
graphGPURiemannTuned.Draw("PL")


tLegend2 = ROOT.TLegend(0.6, 0.15, 0.85, 0.35)
tLegend2.SetFillColor(0)
tLegend2.AddEntry(graphRiemann2, "CPU Riemann", "PL")
tLegend2.AddEntry(graphGPURiemann2, "GPU Riemann", "PL")
tLegend2.AddEntry(graphGPURiemannTuned, "GPU Riemann Tuned", "PL")
tLegend2.Draw()

tCanvas2.SetLogy()
tCanvas2.SetLogx()
tCanvas2.SetTitle("")

input()