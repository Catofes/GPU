import ROOT
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", help="input file")
    args = parser.parse_args()
    f = ROOT.TFile(args.input, "READ")
    t = f.DebugInfo
    data = [0, 0, 0, 0, 0]
    n = [0, 0, 0, 0, 0]
    for entry in t:
        data[int(entry.type)] += entry.total
        n[int(entry.type)] += 1
    for i in range(0, 5):
        if n[i] > 0:
            data[i] /= n[i]
    print(data[0])
    print(data[1])
    print(data[2])
    print(data[3])
    print(data[4])
