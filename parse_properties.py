#!/usr/bin/python3

import bs4 #beautifulSoup
import sys
from props import props

add_props = {
    'displayShowBack' : [ 'display/showBack', 
                          'AOGProperty property_displayShowBack("display/showBack",false);',
                          'extern AOGProperty property_displayShowBack;' ]
}

def parse_settings(file):
    cpp = []
    h = []
    preamble = []

    with file:
        parser = bs4.BeautifulSoup(file.read(),'lxml-xml')
        settings = parser.findAll('Setting')
        for s in settings: 
            t = s['Type']
            n = s['Name']
            #print (t)
            if t == 'System.Int32' or \
               t == 'System.Double' or \
               t == 'system.Decimal' or \
               t == 'System.Byte':
                default_value = s.Value.contents[0]
            elif t == 'System.Boolean':
                default_value = s.Value.contents[0].lower()
            elif t == 'System.String' and n == 'setTool_zones':
                preamble.append('QVector<int> default_zones = { ' + s.Value.contents[0] + ' };')
                default_value = 'QVariant::fromValue(default_zones)'
            elif t == 'System.String' and n == 'setRelay_pinConfig':
                preamble.append('QVector<int> default_relay_pinConfig = { ' + s.Value.contents[0] + ' };')
                default_value = 'QVariant::fromValue(default_relay_pinConfig)'
            elif t == 'System.String' or \
                 t == 'AgOpenGPS.TBrand' or \
                 t == 'AgOpenGPS.HBrand' or \
                 t == 'AgOpenGPS.WDBrand' or \
                 t == 'AgOpenGPS.CFeatureSettings':
                c = s.Value.contents
                if c:
                    default_value = '"' + s.Value.contents[0] + '"'
                else:
                    default_value = '""'

            elif t == 'System.Drawing.Point':
                default_value = 'QPoint(%s)' % s.Value.contents[0]
            elif t == 'System.Drawing.Color':
                fields = s.Value.contents[0].split(',')
                if len(fields) > 1:
                    default_value = 'QColor(%s)' % s.Value.contents[0]
                else:
                    default_value = 'QColor("%s")' % s.Value.contents[0]
            else:
                if s.Value.contents:
                    default_value = '"' + s.Value.contents[0] + '"'
                else:
                    default_value = '""'

            if s['Name'] in props:
                qs_name = props[s['Name']]
            else:
                props[s['Name']] = ''
                qs_name = ""

            cpp.append('AOGProperty property_%s("%s",%s);'% (s['Name'], qs_name, default_value))
            h.append('extern AOGProperty property_%s;' % s['Name'])
            #preamble.extend(cpp)

    return (preamble, cpp, h)

if __name__ == '__main__':
    import argparse

    argparser = argparse.ArgumentParser(prog = sys.argv[0],
                                       description='Parse C# .settings file to create c++ declarations for AOGProperty')
    argparser.add_argument('-c','--cpp', action = "store_true", help = 'Output code for cpp file')
    argparser.add_argument('-i','--header', action = "store_true", help = 'Output header file')
    argparser.add_argument('-d','--dict', action = "store_true", help = 'output python dict of names to help with this script.')

    argparser.add_argument('settings_file', help = 'path to AOG C# Settings.settings file')

    args = argparser.parse_args()
    
    cpp_pre,cpp,h = parse_settings(open(args.settings_file,'r'))

    if (args.header):
        print ('#ifndef PROPERTIES_H')
        print ('#define PROPERTIES_H')
        print ()
        print ('#include "aogsettings.h"')
        print ()
        for line in h:
            print (line)

        for i in add_props:
            print (add_props[i][2])

        print ()

        print ('#endif // PROPERTIES_H')

    elif args.dict:
        import pprint
        print("props = ", end='')
        pprint.pprint (props, sort_dicts = False)
        

    else: 
        for line in cpp_pre:
            print (line)

        for line in cpp:
            print (line)


        for i in add_props:
            print(add_props[i][1])

