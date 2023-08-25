import os
import glob
from func import pipeline, write_result


if __name__ == "__main__":

    fps = glob.glob("*.pcn")
    fps.sort()

    fp=None
    for fp in fps:
        savepath = os.path.basename(fp)[:-4] + "answer.cubes"
        result = pipeline(fp)
        write_result(result, savepath)
        