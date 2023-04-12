fileToParse=$1
name=$2
resFolder="${PWD}/res/"

awk -f ${resFolder}/parse_nbReq.awk ${fileToParse}> ${resFolder}/${name}_nbReqs.csv
awk -f ${resFolder}/parseComDurs.awk ${fileToParse} > ${resFolder}/${name}_comDurs.csv
awk -f ${resFolder}/parseEnergy.awk ${fileToParse} > ${resFolder}/${name}_energy.csv
awk -f ${resFolder}/parseExecDurs.awk ${fileToParse} > ${resFolder}/${name}_execDurs.csv
awk -f ${resFolder}/parseLoad.awk ${fileToParse} > ${resFolder}/${name}_load.csv
awk -f ${resFolder}/parseEndToEnd.awk ${fileToParse} > ${resFolder}/${name}_EndToEnd.csv
awk -f ${resFolder}/parse_nbInst.awk ${fileToParse} > ${resFolder}/${name}_nbInst.csv
awk -f ${resFolder}/parse_reqTot.awk ${fileToParse} > ${resFolder}/${name}_reqTot.csv