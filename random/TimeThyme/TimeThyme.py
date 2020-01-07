#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import time, calendar

class RecordManager:
    def __init__(self):
        self.filename = "records"
        self.records = {}
        self.load()

    def load(self):
        with open(self.filename, "a+") as fh:
            fh.seek(0)
            for line in fh:
                line = line.strip()
                sections = line.split(",")
                mode = sections[1]
                if mode not in self.records: self.records[mode] = []
                self.records[mode].append(line)

    def save(self):
        with open(self.filename, "w+") as fh:
            for record in self.records:
                for string in self.records[record]:
                    fh.write(string + "\n")

    def addRecord(self, mode): self.records[mode] = []
    def deleteRecord(self, mode): self.records.pop(mode, None)
    def getCustoms(self): return sorted(self.records.keys())

    def generateRecord(self, mode):
        print("Press Enter to start", end = "")
        input(); beg = time.time()
        print("Press Enter to stop", end = "")
        input(); end = time.time()

        delta = float("%.2f" % (end - beg))
        epoch = calendar.timegm(time.gmtime())

        record = "{},{},{}".format(epoch, mode, delta)
        print("Generated record: {}".format(record))
        if mode not in self.records: self.records[mode] = []
        self.records[mode].append(record)

    def listRecord(self, mode):
        print()
        for record in self.records[mode]:
            sections = record.split(",")
            epoch = sections[0]
            delta = sections[2]
            print("Epoch {} = {}".format(epoch, delta))

    def moveRecord(self, mode, newMode):
        tmp = []
        for record in self.records[mode]:
            sections = record.split(",")
            final = "{},{},{}".format(sections[0], newMode, sections[2])
            tmp.append(final)
        self.records[newMode] = tmp
        self.deleteRecord(mode)

    def renderRecord(self, mode):
        try:
            records = self.records[mode]
            x = []
            y = []
            for record in records:
                sections = record.split(",")
                epoch = sections[0]
                delta = sections[2]
                x.append(epoch)
                y.append(delta)

            plt.scatter(x, y)
            plt.show()
        except:
            pass

class MenuSystem:
    def __init__(self):
        self.recordManager = RecordManager()

    def query(self):
        print("\nOptions, touch, ls, mv, rm, render, gen, exit (Save), quit (Don't Save)\n->\t", end = "")
        for custom in self.recordManager.getCustoms(): print("{}\t".format(custom), end = "")
        print("\n\t: ", end = "")

        answer = input()
        sections = answer.split(" ")
        action = None
        mode = None
        data = None
        if len(sections) >= 1: action = sections[0]
        if len(sections) >= 2: mode = sections[1]
        if len(sections) >= 3: data = sections[2]
        exists = False
        if mode in self.recordManager.records: exists = True
        newExists = False
        if action == "mv" and data in self.recordManager.records: newExists = True

        if action is not None and mode is not None:
            if action == "touch" and exists == False: self.recordManager.addRecord(mode)
            if action == "rm" and exists == True: self.recordManager.deleteRecord(mode)
            if action == "ls" and exists == True: self.recordManager.listRecord(mode)
            if action == "mv" and exists == True and newExists == False: self.recordManager.moveRecord(mode, data)
            if action == "render" and exists == True: self.recordManager.renderRecord(mode)
            if action == "gen" and exists == True: self.recordManager.generateRecord(mode)
        if action == "exit": self.recordManager.save(); exit()
        if action == "quit": exit()

        self.query()

ms = MenuSystem()
ms.query()
