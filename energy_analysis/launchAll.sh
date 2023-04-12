buildDir="/home/clem/Code/github.com/klementc/internship_simgrid/build"
platfFiles=("platf_test2_cloudclose_2ms.xml" "platf_test2_cloudfar_2ms.xml" "platf_test2_cloudveryclose_2ms.xml" "platf_test2_cloudclose_5ms.xml" "platf_test2_cloudfar_5ms.xml" "platf_test2_cloudveryclose_5ms.xml")
prefixes=("close_2ms" "far_2ms" "veryclose_2ms" "cloudclose_5ms" "cloudfar_5ms" "cloudveryclose_5ms")
appType=("exec" "comm")

for platf in ${!platfFiles[@]};
do
  for appT in ${!appType[@]};
  do
    # Run cloud experiment
    ${buildDir}/energy_analysis/test_2_cloud_${appType[$appT]} "./platforms/${platfFiles[platf]}" "./config/test_2_cloud.config" "./config/test_2_elasticPool_cloud.config" 1 --log="elasticInstance.thres:debug elastic.thres:debug dataSource.thres:debug run_log.thres:debug elasticPolicyCPU.thres:debug" > outCloud 2>&1
    # Run fog experiment
    ${buildDir}/energy_analysis/test_2_fog_${appType[$appT]} "./platforms/${platfFiles[platf]}" "./config/test_2_fog.config" "./config/test_2_elasticPool_fog.config" 1 --log="elasticInstance.thres:debug elastic.thres:debug dataSource.thres:debug run_log.thres:debug elasticPolicyCPU.thres:debug" > outFog 2>&1
    # Generate analysis
    bash analyse.sh outFog "${prefixes[$platf]}_fog_${appType[$appT]}_d32"
    bash analyse.sh outCloud "${prefixes[$platf]}_cloud_${appType[$appT]}_d32"
    echo "Finished ${prefixes[$platf]}"
    echo "Generate plots"
    Rscript genPlot.R "${prefixes[$platf]}_fog_${appType[$appT]}_d32"
    evince "res/${prefixes[$platf]}_fog_${appType[$appT]}_d32_summary.pdf" &
    Rscript genPlot.R "${prefixes[$platf]}_cloud_${appType[$appT]}_d32"
    evince "res/${prefixes[$platf]}_cloud_${appType[$appT]}_d32_summary.pdf" &
  done
done
