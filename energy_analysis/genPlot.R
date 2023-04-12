options(warn=-1)
library(plyr); library(dplyr)
library(ggplot2)
library(tidyr)
#library(tikzDevice)
library(gridExtra)
library(ggpubr)
library(plotly)
library(ggquadrilateral)
library(cowplot)
library(ggsci)
library(reshape2)
library(stringr)

setwd("/home/clem/Code/gitlab.inria.fr/clement/wifi_paper_use_cases/microservices/energy_analysis/")
args = commandArgs(trailingOnly=TRUE)
prefix<-args[1]
d_comDur <- as_tibble(read.csv(paste("./res/",prefix,"_comDurs.csv", sep="")))
d_energy <- as_tibble(read.csv(paste("./res/",prefix,"_energy.csv", sep="")))
d_execDur <- as_tibble(read.csv(paste("./res/",prefix,"_execDurs.csv", sep="")))
d_load <- as_tibble(read.csv(paste("./res/",prefix,"_load.csv", sep="")))
d_nbReqs <- as_tibble(read.csv(paste("./res/",prefix,"_nbReqs.csv", sep="")))
d_endToEnd <- as_tibble(read.csv(paste("./res/",prefix,"_EndToEnd.csv", sep="")))
d_nbInst <- as_tibble(read.csv(paste("./res/",prefix,"_nbInst.csv", sep="")))
d_reqS <- as_tibble(read.csv(paste("./res/",prefix,"_reqTot.csv", sep="")))

d_load_cloud <- d_load  %>%
   filter(str_detect(host, '^cloud_'))
d_load_edge <- d_load  %>%
   filter(str_detect(host, '^edge-'))
d_load_sta <- d_load  %>%
   filter(str_detect(host, '^STA_'))
#pdf(file= "sample.pdf" )
#par( mfrow= c(4,2) )


d_enR <- d_energy %>% pivot_longer(cols = names(.))
#options(repr.plot.width=15, repr.plot.height=6)
energy_pue_cloud <- (d_enR %>% filter(name=="cloudHostsEnergy"))$value
energy_pue_fog <- (d_enR %>% filter(name=="edgeHostsEnergy"))$value
d_enR <- d_enR %>% add_row(name = "PUECloud", value = energy_pue_cloud*0.1) %>%
                    add_row(name = "PUEFog", value=energy_pue_fog*1)


# compute proportional energy for core routers
capRouterCore <- 0.25 * (48e+9 / 8)
propConsCore  <- (d_reqS$sizeTot/270)/capRouterCore

# energy values by part
energyEndUsers <- d_energy$staHostsEnergy+d_energy$wifiRouterEnergy
energyCloud   <- d_energy$cloudHostsEnergy+ d_energy$cloudLinkEnergy
energyEdge    <- d_energy$edgeHostsEnergy + d_energy$edgeLinkEnergy
energyNetwork <- (propConsCore*d_energy$coreRouterEnergy)+ d_energy$backboneLinkEnergy + d_energy$collectorLinkEnergy + d_energy$coreLinkEnergy+d_energy$wifiLinkEnergy
energyCloudPUE <- 1.1 * energyCloud
energyEdgePUE     <- 1.7 * energyEdge
dfEnByPart <- data.frame(energyEndUsers = c(energyEndUsers),
                          energyCloudPUE = c(energyCloud),
                          energyEdgePUE = c(energyEdge),
                          energyNetwork = c(energyNetwork)
              )
dfEnByPartR <- dfEnByPart %>% pivot_longer(cols = names(.))

dfEnByPartR <- dfEnByPartR %>% filter(value!=0)
p0 <- ggplot(dfEnByPartR, aes(x=1.5, y=value, fill=name)) +
      geom_bar(stat="identity", width=1, color="white") +
      coord_polar("y", start=0) +
      annotate("text", x=0, y=0, label=paste("Total: ",round(sum(dfEnByPartR$value)), "J",sep=""), color="black", size=12)+
      theme_void()+   geom_text(aes(y = value/3 + c(0, cumsum(value)[-length(value)]),
            label = paste(round(value),"J",sep="")), size=10)
p0
ggsave(file=paste("visualize/img/",prefix,"_energy.svg",sep=""), plot=p0, width=20, height=10)


p01<-ggplot(data=dfEnByPartR, aes(x=name, y=value)) +
  geom_bar(stat="identity")+
  geom_text(aes(label=round(value), y=0), color="red", vjust=0) +
  theme_bw()+
  ggtitle("Energy of components (coreProportional)")+ theme(plot.title = element_text(size=16), axis.text.x = element_text(angle = 45, hjust=1),text=element_text(size=21))
p01
ggsave(file=paste("visualize/img/",prefix,"_energy_2.svg",sep=""), plot=p01, width=10, height=8)

p1<-ggplot(data=d_enR, aes(x=name, y=value)) +
  geom_bar(stat="identity")+
  geom_text(aes(label=value, y=0), color="red", vjust=0) +
  theme_bw()+
  ggtitle("Energy of components")+ theme(plot.title = element_text(size=16), axis.text.x = element_text(angle = 45, hjust=1))
p1

p2 <- ggplot(d_enR, aes(x="", y=value, fill=name)) +
  geom_bar(stat="identity", width=1, color="white") +
  coord_polar("y", start=0) +
  annotate("text", x=0, y=0, label=paste("Energy tot:",sum(d_enR$value)), color="red", size=8)+

  theme_void() # remove background, grid, numeric labels
p2

set.seed(1234)
p3 <- ggplot(d_execDur, aes(x=start, y=dur)) +
        geom_bin2d() +
        scale_fill_distiller(palette="Spectral", direction=-1) +
        theme_bw()+theme(text=element_text(size=21))+
        ggtitle("Exec Duration")
p3
ggsave(file=paste("visualize/img/", prefix,"_execDur.svg",sep=""), plot=p3, width=10, height=5)

p4 <- ggplot(d_comDur, aes(x=start, y=dur)) +
        geom_bin2d() +
        scale_fill_distiller(palette="Spectral", direction=-1) +
        theme_bw()+theme(text=element_text(size=21))+
        ggtitle("Communication Duration")
p4
ggsave(file=paste("visualize/img/",prefix,"_comDur.svg",sep=""), plot=p4, width=10, height=5)

p5 <- ggplot(d_load_cloud, aes(x=start, y=avg, group=host, color=host)) +
  geom_point(size=2)+
  geom_line()+
  theme_bw()+theme(text=element_text(size=21))+
  ggtitle("Cloud Hosts Loads")+ theme(text=element_text(size=21),plot.title = element_text(size=16), legend.position="none")
p5
ggsave(file=paste("visualize/img/",prefix,"_cloudLoad.svg",sep=""), plot=p5, width=10, height=8)

p6 <- ggplot(d_load_edge, aes(x=start, y=avg, group=host, color=host)) +
  geom_point(size=2)+
  geom_line()+
  theme_bw()+
  ggtitle("Edge Hosts Loads")+ theme(text=element_text(size=21),plot.title = element_text(size=16), legend.position="none")
p6
ggsave(file=paste("visualize/img/",prefix,"_edgeLoad.svg",sep=""), plot=p6, width=10, height=8)

p7 <- ggplot(d_load_sta, aes(x=start, y=avg, group=host, color=host)) +
  geom_point(size=2)+
  geom_line()+
  theme_bw()+
  ggtitle("STA Hosts Loads")+ theme(text=element_text(size=21),plot.title = element_text(size=16), legend.position="none")
p7
ggsave(file=paste("visualize/img/",prefix,"_staLoad.svg",sep=""), plot=p7, width=10, height=8)

p8 <- ggplot(data=d_nbReqs, aes(x=host, y=finished)) +
  geom_bar(stat="identity")+
  theme_bw()+
  ggtitle("Requests successfully executed")+ theme(text=element_text(size=21),plot.title = element_text(size=16))
p8
ggsave(file=paste("visualize/img/",prefix,"_reqFinished.svg",sep=""), plot=p8, width=10, height=4)

p9 <- ggplot(data=d_endToEnd, aes(x=end, y=dur)) +
  geom_bin2d() +
  scale_fill_distiller(palette="Spectral", direction=-1) +
  theme_bw()+theme(text=element_text(size=21))+
  ggtitle("End-To-End Durations")
ggsave(file=paste("visualize/img/",prefix,"_endToEndDur.svg",sep=""), plot=p9, width=10, height=8)

p10 <- ggplot(data=d_nbInst, aes(x=ts, y=nbInst, color=host)) +
  geom_point()+
  geom_line()+
  theme_bw()+theme(text=element_text(size=21))+
  ggtitle("Nb Instances")+ theme(plot.title = element_text(size=16), legend.position="none")
p10
ggsave(file=paste("visualize/img/",prefix,"_nbInst.svg",sep=""), plot=p10, width=10, height=5)

d_endToEndf <- d_endToEnd %>% filter(end>150)
p11 <- ggplot(data=d_endToEndf, aes(x=end, y=dur)) +
        geom_bin2d() +
        scale_fill_distiller(palette="Spectral", direction=-1) +
        theme_bw()+
  ggtitle("End-To-End Durations past 150")
p11
ggsave(file=paste("visualize/img/",prefix,"_endToEndDur2.svg",sep=""), plot=p11, width=11, height=8)

d_nbInst_grouped <- d_nbInst %>% group_by(ts) %>% mutate(sum = sum(nbInst))
p12 <- ggplot(data=d_nbInst_grouped, aes(x=ts, y=sum)) +
  geom_point()+
  geom_line()+
  theme_bw()+
  ggtitle("Nb active instances tot")+ theme(plot.title = element_text(size=16), legend.position="none",text=element_text(size=21))
p12
ggsave(file=paste("visualize/img/",prefix,"_nbInstTot.svg",sep=""), plot=p12, width=10, height=5)

# CO2 section
co2peryearfactor <- (3600*24*365)/270
dfCO2 <- data.frame(matrix(ncol = 4, nrow = 0))
colnames(dfCO2) <- c("country","co2EndUsers","co2Cloud","co2Edge")
# france
dfCO2[nrow(dfCO2) + 1, ] <- c("France",co2peryearfactor*energyEndUsers/3600000*56,
                                    co2peryearfactor*energyCloud/3600000*56,
                                    co2peryearfactor*energyEdge/3600000*56)
dfCO2[nrow(dfCO2) + 1, ] <- c("Spain",co2peryearfactor*energyEndUsers/3600000*141,
                                    co2peryearfactor*energyCloud/3600000*141,
                                    co2peryearfactor*energyEdge/3600000*141)
dfCO2[nrow(dfCO2) + 1, ] <- c("Great-Britain",co2peryearfactor*energyEndUsers/3600000*184,
                                    co2peryearfactor*energyCloud/3600000*184,
                                    co2peryearfactor*energyEdge/3600000*184)
dfCO2[nrow(dfCO2) + 1, ] <- c("USA",co2peryearfactor*energyEndUsers/3600000*388,
                                    co2peryearfactor*energyCloud/3600000*388,
                                    co2peryearfactor*energyEdge/3600000*388)
pco2_1<-ggplot(data=dfCO2, aes(x=country, y=as.numeric(co2EndUsers)/1000)) +
  geom_bar(stat="identity")+
  #geom_text(aes(label=round(as.numeric(co2EndUsers)/1000), y=0), color="", vjust=0, size=10) +
  theme_bw()+
    ylab("CO2eq (kg)")+
  xlab("")+
  ggtitle("CO2 End-Users")+ theme(,text=element_text(size=21), plot.title = element_text(size=16), axis.text.x = element_text(angle = 45, hjust=1))
pco2_2<-ggplot(data=dfCO2, aes(x=country, y=as.numeric(co2Edge)/1000)) +
  geom_bar(stat="identity")+
  #geom_text(aes(label=round(as.numeric(co2Edge)/1000), y=0), color="", vjust=0, size=10) +
  theme_bw()+
    ylab("CO2eq (kg)")+
  xlab("")+
  ggtitle("CO2 Edge")+ theme(,text=element_text(size=21),plot.title = element_text(size=16), axis.text.x = element_text(angle = 45, hjust=1))
pco2_3<-ggplot(data=dfCO2, aes(x=country, y=as.numeric(co2Cloud)/1000)) +
  geom_bar(stat="identity")+
  #geom_text(aes(label=round(as.numeric(co2Cloud)/1000), y=0), color="", vjust=0, size=10) +
  theme_bw()+
    ylab("CO2eq (kg)")+
  xlab("")+
  ggtitle("CO2 Cloud")+ theme(,text=element_text(size=21), plot.title = element_text(size=16), axis.text.x = element_text(angle = 45, hjust=1))
pco2 <- grid.arrange(pco2_1, pco2_2, pco2_3, ncol=1, nrow = 3)
ggsave(file=paste("visualize/img/",prefix,"_co2.svg",sep=""), plot=pco2, width=10, height=13)





pdf(file= paste("respdf/",prefix,"_summary.pdf", sep=""), width=15,height=17)
p <- ggarrange(p01, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, ncol=2, nrow=7,
          common.legend = FALSE)
annotate_figure(p, top = text_grob(paste("Scenario: ",prefix,end=""),
               color = "red", face = "bold", size = 18))
dev.off()
