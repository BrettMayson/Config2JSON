# Config2JSON

Config2JSON is the fastest and most reliable way to convert mission sqm files, or cpp config files to JSON for use in other programs or websites.

## Comparision
Raw: Straight output from either a config dump, or DePBO programs.  
Cleaned: Comments and defines removed. Special characters removed from class names.

| Tool                                                                 | Raw Mission SQM  (55.1 kB) | Cleaned Mission SQM (54.7 kB) | Raw All in One Config (66.2 MB) | Cleaned All in One Config (66.2 MB) |
|----------------------------------------------------------------------|----------------------------|-------------------------------|---------------------------------|-------------------------------------|
| [arma-class-parser](https://github.com/Fusselwurm/arma-class-parser) | Crash                      | 0.07 Seconds                  | Crash                           | 16.45 Seconds                       |
| [sqm2json](https://github.com/nicolasFlinois/sqm2json)               | Crash                      | Crash                         | Crash                           | Crash                               |
| [Config2JSON](https://github.com/brettmayson/Config2JSON)            | < 0.01 Seconds             | < 0.01 Seconds                | 2.18 Seconds                    | 2.18 Seconds                        |
