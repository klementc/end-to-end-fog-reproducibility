buildDir="/home/clem/Code/gitlab.inria.fr/clement/wifi_paper_use_cases/microservices/build/"
#buildDir="/home/ccourageuxsudan/wifi_paper_use_cases/microservices/build/"

appType=("14APPS_video_fog" "14APPS_video_fog_coloc")
confAppT=("../energy_analysis/config/14APPS_fog.config" "../energy_analysis/config/14APPS_fog_coloc.config")
confPoolT=("../energy_analysis/config/14APPS_fog_elasticpool.config" "../energy_analysis/config/14APPS_fog_elasticpool_coloc.config")

lat=("2ms" "5ms")
netSize=("10000.0" "10000.0" "125000.0")
execR=("2" "1" "10")

for appT in ${!appType[@]};
do
  for l in ${!lat[@]};
  do
    for netS in ${!netSize[@]};
    do
      suffix="fog_${appType[$appT]}_${lat[$l]}_${netSize[$netS]}_${execR[$netS]}"
      #echo ${suffix}
      # Run Cloud experiment TCP
      #${buildDir}/energy_analysis/${appType[$appT]} ../energy_analysis/platforms/platf_16FOG_${lat[$l]}.xml ${confAppT[$appT]} ${confPoolT[$appT]} ${execR[$netS]} ${netSize[$netS]} --log="elasticInstance.thres:debug elastic.thres:debug dataSource.thres:debug run_log.thres:debug elasticPolicyCPU.thres:debug" > out_${suffix}_tcp 2>&1
      # Run Cloud experiment UDP
      #${buildDir}/energy_analysis/${appType[$appT]} ../energy_analysis/platforms/platf_16FOG_${lat[$l]}.xml ${confAppT[$appT]} ${confPoolT[$appT]} ${execR[$netS]} ${netSize[$netS]} --log="elasticInstance.thres:debug elastic.thres:debug dataSource.thres:debug run_log.thres:debug elasticPolicyCPU.thres:debug" --cfg="network/TCP-gamma:0"> out_${suffix}_udp 2>&1

      # Generate analysis
      bash analyse.sh out_${suffix}_tcp "out_${suffix}_tcp"
      echo "Finished ${prefixes[$platf]}"
      echo "Generate plots"
      Rscript genPlot.R "out_${suffix}_tcp"
      #evince "respdf/out_${suffix}_tcp_summary.pdf" &

      bash analyse.sh out_${suffix}_udp "out_${suffix}_udp"
      echo "Finished ${prefixes[$platf]}"
      echo "Generate plots"
      Rscript genPlot.R "out_${suffix}_udp"
      #evince "respdf/out_${suffix}_udp_summary.pdf" &
    done
  done
done
