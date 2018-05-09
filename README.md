# Config2JSON

Config2JSON is the fastest and most reliable way to convert mission sqm files, or cpp config files to JSON for use in other programs or websites.

## Comparision
Raw: Straight output from either a config dump, or DePBO programs.  
Cleaned: Comments and defines removed. Special characters removed from class names.

Values are averaged from 10 runs.

| Tool                                                                 | Raw Mission SQM  (55.1 kB) | Cleaned Mission SQM (54.7 kB) | Raw All in One Config (66.2 MB) | Cleaned All in One Config (66.2 MB) |
|----------------------------------------------------------------------|----------------------------|-------------------------------|---------------------------------|-------------------------------------|
| [arma-class-parser](https://github.com/Fusselwurm/arma-class-parser) | Crash                      | 0.07 Seconds                  | Crash                           | 16.52 Seconds                       |
| [sqm2json](https://github.com/nicolasFlinois/sqm2json)               | Crash                      | Crash                         | Crash                           | Crash                               |
| [Config2JSON](https://github.com/brettmayson/Config2JSON)            | < 0.01 Seconds             | < 0.01 Seconds                | 1.89 Seconds                    | 1.91 Seconds                        |

## Usage
Config2JSON uses only standard C++ and has been tested with the 2014 C++ Standard (201402L) using G++ 7.2.0.

Config2JSON can be built on Linux using

    g++ config2json.cxx -o config2json

or

    make

and ran using

    ./config2json input.sqm output.json
