#!/usr/bin/env python

#written for Python3.4

import re
import sys
from datetime import datetime, timedelta
try:
    import statistics
except ImportError:
    import stats as statistics
import math

# http://mathforum.org/library/drmath/view/60969.html
def lower_quartile(values):
    # please sort values beforehand!
    try:
        return values[math.ceil( ( len(values) + 1 ) / 4.0 ) - 1 ]
    except:
        import pdb; pdb.set_trace()
def upper_quartile(values):
    # please sort values beforehand!
    return values[math.floor( ( len(values) + 1 ) / 4.0 * 3.0 ) - 1]

def read_logfile(file_handle, out_file, line_filter, oldest, newest, weather, pool):
    r = re.compile(line_filter)

    pooling_key = {'days': '%Y-%m-%d', 'weeks': '%Y-W%W-1', 'months': '%Y-%m'}[pool]

    data = dict()
    for line in file_handle:
        timestamp = datetime.strptime(line[0:19], "%Y-%m-%d_%H:%M:%S")
        datestamp = timestamp.date()
        if weather and timestamp < timestamp.replace(hour=9, minute=0, second=0):
            datestamp -= timedelta(days=1)
        if newest and datestamp > newest: continue
        if oldest and datestamp < oldest: continue
        #timestamp.replace(hour=0, minute=0, second=0)
        key = datestamp.strftime(pooling_key)
        if line_filter in line:
            out_file.write(line)
        result = r.search(line)
        if result:
            try:
                data[key]
            except:
                data[key] = []
            try:
                data[key].append(float(result.groups()[0]))
            except:
                print("cannot parse line", line)
                print(result.groups())
                continue
    if len(data) == 0: return
    out_file.write("# date whisker_low box_low median box_high whisker_high mean samples\n")
    sorted_keys = [key for key in data]
    sorted_keys.sort()
    for key in sorted_keys:
        data[key].sort()
        entry = dict()
        if pool == 'weeks':
            entry['date'] = datetime.strptime(key,'%Y-W%W-%w').strftime('%Y-%m-%d')
        else:
            entry['date'] = key
        entry['whisker_low'] = min(data[key])
        entry['box_low'] = lower_quartile(data[key])
        entry['median'] = statistics.median(data[key])
        entry['box_high'] = upper_quartile(data[key])
        entry['whisker_high'] = max(data[key])
        entry['mean'] = statistics.mean(data[key])
        entry['samples'] = len(data[key])
        out_file.write("{date} {whisker_low} {box_low} {median} {box_high} {whisker_high} {mean} {samples}\n".format(**entry))

def date_parser(string):
    return datetime.strptime(string, '%Y-%m-%d').date()

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Parse FHEM log files.')
    parser.add_argument('logfile', type=argparse.FileType('r'))
    parser.add_argument('outfile', nargs='?', type=argparse.FileType('w'), default=sys.stdout)
    parser.add_argument('--filter', default=None)
    parser.add_argument('--oldest', type=date_parser, default=None)
    parser.add_argument('--newest', type=date_parser, default=None)
    parser.add_argument('--weather', action='store_true', help='Assign data between 0:00 a.m. and 9:00 a.m. to the previous day')
    parser.add_argument('--course', action='store_true', help="Don't output statistics, print the values")
    parser.add_argument('--pool', choices=['days','weeks','months'], default='days', help="Calculate statistics from pooled (days, weeks or months) [default: days]")

    args = parser.parse_args()

    read_logfile(args.logfile, args.outfile, args.filter, args.oldest, args.newest, args.weather, args.pool)

if __name__ == "__main__":
    main()

