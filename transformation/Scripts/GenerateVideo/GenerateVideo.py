import subprocess as sub
import shutil
import os
import sys

import LayoutGenerators


def GenerateConfigString(layouts_a, fps, bitrate, nbFrames, inputVideo, output):
    c = '[Global]\n'
    c += 'fps= {}\n'.format(fps)
    c += 'layoutFlow= [[ "{}"'.format(inputVideo)
    for (l,a) in layouts_a:
        c += ', "{}"'.format( l.GetName() )
        first = False
    c += ']]\n'
    c += 'displayFinalPict=false\n'
    c += 'videoOutputName= {}\n'.format(output)
    c += 'videoOutputBitRate={}\n'.format(bitrate)
    c += 'qualityOutputName=\n'
    c += 'nbFrames={}\n'.format(nbFrames)

    for (l,a) in layouts_a:
        c+= l.GenerateLayout(a)
    return c

def GenerateVideo(config, trans, layouts_a, fps, nbFrames,  inputVideo, outputId, bitrate=0):
    tmpVideo = '/tmp/tmp.mkv'
    lastName = None
    lastLayout = None
    lastA = None
    for (l,a) in layouts_a:
        lastName = l.GetName()
        lastLayout = l
        lastA = a
    try:
        with open(config, 'w') as cf:
            cf.write(GenerateConfigString(layouts_a, fps, bitrate, nbFrames, inputVideo, tmpVideo))

        sub.check_call([trans, '-c', config])

        shutil.move('/tmp/tmp1{}.mkv'.format(lastName), '{}.mkv'.format(outputId))
        shutil.copy(config, '{}_log.txt'.format(outputId))

    except KeyboardInterrupt:
        print('Received <ctrl-c>')
        sys.exit(1)
    except Exception as inst:
        print (inst)
    finally:
        if os.path.isfile('/tmp/tmp1{}.mkv'.format(lastName)):
            os.remove('/tmp/tmp1{}.mkv'.format(lastName))
        return LayoutGenerators.LayoutStorage(lastLayout, lastA, nbFrames)
