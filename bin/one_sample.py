import graphmod
import argparse
import logging
#import yep


logging.basicConfig(format='%(asctime)s %(message)s', datefmt='%Y-%m-%d %H:%M:%S', level=logging.INFO)

logging.info("starting")

parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input", dest="input")
parser.add_argument("-o", "--output", dest="output")
parser.add_argument("-s", "--samples", dest="samples", type=int, default=1)
parser.add_argument("-e", "--every", dest="every", type=int, default=5)
options = parser.parse_args()

fg = graphmod.FactorGraph(options.input)
logging.info(fg)

s = graphmod.SerialSampler()

logging.info("sampling")
#yep.start("debug.txt")
s.sample(fg)
#yep.stop()
logging.info("ending")
#print fg.perplexity(["gr"])

#fg.save(options.output)
#logging.info("done")
