import graphmod
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input", dest="input")
parser.add_argument("-o", "--output", dest="output")
options = parser.parse_args()

fg = graphmod.FactorGraph(options.input)
s = graphmod.SerialSampler()
s.sample(fg)
print fg.perplexity(["gr"])
fg.save(options.output)
