import ROOT

data = '''
1000	2000	5000	10000	20000	50000	100000	200000	500000	1000000	2000000	5000000	10000000	20000000	50000000	100000000	200000000
237347.5352	477346.2078	1180622.82	2359769.724	4743672.789	11642909.31	23236435.41	46363395.35	116394465.5	233281090.6							
164701.3639	331171.279	820183.7044	1646363.02	3286950.909	8211757.316	16387862.79	32788333.87	81793799.9	163761205.2							
677170.8065	670512.9781	681801.5873	671672.3518	689400.1411	681922.5521	677230.1758	698317.747	708469.6118	1093248.443	1855289.817	4197851.292	8654051.257	16018435.87	40395855.12	90392631.05	162421769.8
676947.0323	670534.8703	679443.6121	672247.566	687328.6633	678100.8933	677834.0261	694619.8749	694064.549	908787.2109	1479214.547	3238553.496	6724302.937	12122568.49	30600717.97	70503813.12	122765455.8
196684.2483	196999.2088	207095.3594	194580.6936	205486.3777	203428.6599	205320.7774	291440.0003	411663.5315	704100.7427	1273253.392	3032314.615	6520269.413	11913798.28	30392659.03	70299273.66	122552100.6'''

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
tLegend.AddEntry(graphGauss, "Gauss", "PL")
tLegend.AddEntry(graphRiemann, "Riemann", "PL")
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

graphGPURiemann2 = ROOT.TGraph(len(GPURiemann))
for i in range(0, len(GPURiemann)):
    graphGPURiemann2.SetPoint(i, X[i], GPURiemann[i])
graphGPURiemann2.SetTitle("GPU Riemann")
graphGPURiemann2.SetLineColor(8)
graphGPURiemann2.SetLineWidth(3)
graphGPURiemann2.SetMarkerStyle(20)
graphGPURiemann2.SetMarkerColor(8)
graphGPURiemann2.SetMarkerSize(1)
graphGPURiemann2.Draw("PL")

graphGPURiemannTuned = ROOT.TGraph(len(GPURiemannTuned))
for i in range(0, len(GPURiemannTuned)):
    graphGPURiemannTuned.SetPoint(i, X[i], GPURiemannTuned[i])
graphGPURiemannTuned.SetTitle("GPU Riemann Tuned")
graphGPURiemannTuned.SetLineColor(49)
graphGPURiemannTuned.SetLineWidth(3)
graphGPURiemannTuned.SetMarkerStyle(21)
graphGPURiemannTuned.SetMarkerColor(49)
graphGPURiemannTuned.SetMarkerSize(1)
graphGPURiemannTuned.Draw("PL")


tLegend2 = ROOT.TLegend(0.2, 0.6, 0.5, 0.8)
tLegend2.SetFillColor(0)
tLegend2.AddEntry(graphGPURiemann2, "GPU Riemann", "PL")
tLegend2.AddEntry(graphGPURiemannTuned, "GPU Riemann Tuned", "PL")
tLegend2.Draw()

tCanvas2.SetLogy()
tCanvas2.SetLogx()
tCanvas2.SetTitle("")

input()