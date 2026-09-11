|            | UniTs          | - University | of Trieste |                 |
| ---------- | -------------- | ------------ | ---------- | --------------- |
| Faculty    | of Scientific  | and Data     | Intensive  | Computing       |
| Department | of mathematics | informatics  |            | and geosciences |
CClloouudd
CCoommppuuttiinngg
Lecturer:
|     | Prof. | Taffoni | Giuliano |     |
| --- | ----- | ------- | -------- | --- |
Author:
|     |     | Christian | Faccio |     |
| --- | --- | --------- | ------ | --- |
|     |     | March 26, | 2025   |     |
ThisdocumentislicensedunderaCreativeCommonsAttribution-NonCommercial-ShareAlike(CC
BY-NC-SA)license. Youmayshareandadaptthismaterial,providedyougiveappropriatecredit,donot
useitforcommercialpurposes,anddistributeyourcontributionsunderthesamelicense.
| Github                     |     |     | Envelope |                            |
| -------------------------- | --- | --- | -------- | -------------------------- |
| github.com/christianfaccio |     |     |          | christianfaccio@outlook.it |

Contents
1 Introduction 1
2 CloudComputingconceptandarchitecture 7
2.1 ServiceAttributes . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 7
2.2 CloudServiceModels . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 9
2.3 CloudDeploymentModels . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 9
2.4 CloudComputingInfrastructure . . . . . . . . . . . . . . . . . . . . . . . . . . 10
2.5 CommunicationinCloudComputing . . . . . . . . . . . . . . . . . . . . . . . . 11
3 Computingbasics 13
3.1 Cluster . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 15
4 Virtualization 18
4.1 VirtualMachines . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
4.2 Virtualizationmodels . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
4.3 Virtualizingcomponents . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
4.3.1 CPU . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
4.3.2 Memory . . . . . . . . . . .D.ra.ft. . . . . . . . . . . . . . . . . . . . . . 20
4.3.3 Network. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21
4.3.4 Disk . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22
4.4 Emulation . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 23
5 BenchmarkingaLinuxplatform 24
5.1 Componentstotest . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 24
5.2 Benchmarkingtools . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 25
5.2.1 HPLinpack(HPL) . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 25
5.2.2 HighPerformanceConjugateGradient(HPCG) . . . . . . . . . . . . . . 25
5.3 Tests . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 26
5.3.1 CPU . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 26
5.3.2 Memory . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 27
5.3.3 DiskI/OandStorage . . . . . . . . . . . . . . . . . . . . . . . . . . . . 28
6 Containers 30
6.1 Properties . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 31
6.2 Docker . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 32
6.2.1 LinuxNamespaces . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 33
6.2.2 LinuxCGroups . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 34
6.2.3 DockerFile . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 35
7 DataCloudandCloudSecurity 37
7.1 DataCloud . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 37
7.1.1 DistributedFileSystem . . . . . . . . . . . . . . . . . . . . . . . . . . 37
Contents i

7.1.2 CloudStoragecapabilities . . . . . . . . . . . . . . . . . . . . . . . . . 41
7.2 CloudSecurity . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 43
Draft
Contents ii

1
Introduction
BOOKDefinition: WhatisComputing?
Computingistheprocessofusingcomputertechnologytocompleteagivengoal-oriented
task. Computing may encompass the design and development of software and hardware
systemsforabroadrangeofpurposes.
Today, each scientific instrument is critically dependent on computing for sensor control, data
processing,internationalcollaboration,andaccess. Computationalmodellinganddataanalytics
areapplicabletoallareasofscienceandengineering. Theabilitytousecomputingeffectivelyisa
keyskillforallscientistsandengineers.
Draft
Figure1.1: Theevolutionofcomputing.
WecannowdefinewhatDistributedComputingis:
BOOKDefinition: DistributedComputing
Adistributedsystemisacollectionofautonomouscomputersthatareinterconnectedwith
eachotherandcooperate,therebysharingresourcessuchasprintersanddatabases.
ADDArchitectureisadistributedsystemthatconsistsofmultipleautonomouscomputersthat
communicate through a computer network. The computers interact with each other in order to
achieve a common goal. It is based on a client-server model, where the client requests services
fromtheserver.
Chapter1. Introduction 1

Figure1.2: DistributedComputing.
Thereisalsoa3-tierarchitecture,wheretheclientinteractswiththeserver,whichinteractswith
thedatabase.
Draft
Figure1.3: 3-tierarchitecture.
And finally a peer-to-peer architecture, where each computer can act as a client or a server.
Responsibilitiesareuniformlydividedamongallmachines.
Figure1.4: Peer-to-peerarchitecture.
Chapter1. Introduction 2

Question-CircleExample: Hadoop
Hadoopisanopen-source softwareframeworkfor storingdataandrunningapplicationson
clustersofcommodityhardware. It providesmassivestoragefor anykindofdata, enormous
processing power and the ability to handle virtually limitless concurrent tasks or jobs. It
implementsadistributedscalablecomputingmodelfordataanalytics,usingaDistributed
FileSystem(HDFS)anda MapReduceprogrammingmodel.
• HDFS: Hadoop Distributed File System, a distributed file system that provides high-
throughputaccesstoapplicationdata. Itmanagesalargenumberoflargefiles,distributing
themacrossthenodesinacluster.
• MapReduce: aprogrammingmodelforprocessingandgeneratinglargedatasetswitha
parallel,distributedalgorithmonacluster.
A Computer Cluster is a group of linked computers, working together closely so that in many
respectstheyformasinglecomputer. Thecomponentsofaclusterarecommonly,butnotalways,
connectedtoeachotherthroughfastlocalareanetworks. Clustersareusuallydeployedtoimprove
performance and availability over that of a single computer, while typically being much more
cost-effectivethansinglecomputersofcomparablespeedoravailability.
Draft
Figure1.5: ComputerCluster.
Aclustercanbeaccessedthrough abatch system,which isa softwaresystem usedtomanageand
schedulebatchjobs. Batchsystemsareusedinenvironmentswhereusersdonotinteractivelyusea
computersystem,butinsteadenterasetofcommandstobeexecutedbythesystematalatertime.
Thefilesystemstructureissharedamongallnodesinthecluster.
Chapter1. Introduction 3

| High    | Availability |     | Cluster | Network | Load Balancing |     | HPCCluster |     |
| ------- | ------------ | --- | ------- | ------- | -------------- | --- | ---------- | --- |
| (Linux) |              |     |         | Cluster |                |     |            |     |
Mission-criticalapplications Operate by distributing Low-latencynetwork
|     |     |     |     | a workload | evenly | over |     |     |
| --- | --- | --- | --- | ---------- | ------ | ---- | --- | --- |
multiplebackendnodes
High-availabilityclusters(aka Typically, the cluster will be Message-passinglibraries
| Failover                 | Clusters) |             | are imple- | configured | with multiple  | re-   |     |     |
| ------------------------ | --------- | ----------- | ---------- | ---------- | -------------- | ----- | --- | --- |
| mentedforthepurposeofim- |           |             |            | dundant    | load-balancing | front |     |     |
| provingtheavailabilityof |           |             | ser-       | ends       |                |       |     |     |
| vices                    | which     | the cluster | pro-       |            |                |       |     |     |
vides
Provideredundancy All available servers process Parallelfilesystem
requests
Eliminate singlepoints of fail- Webservers,mailservers,etc. HPCsystemsoftware
ure
|     |                 |     |          | Table1.1: | ClassificationofClusters |     |     |     |
| --- | --------------- | --- | -------- | --------- | ------------------------ | --- | --- | --- |
|     | EYEObservation: |     | HPCvsHTC |           |                          |     |     |     |
High-PerformanceComputing(HPC)istheuseofparallelprocessingforrunningadvanced
applicationprogramsefficiently,reliablyandquickly. Thetermappliesespeciallytosystems
thatfunctionaboveateraflopor1012 floating-pointoperationspersecond. ThetermHigh-
ThroughputComputing (HTC)referstotheuseofmanycomputingresourcesover long
periodsoftimetoaccomplishacomputationaltask.
Draft
• The computingchallenge: In order to toimprove the codes’performance, multi (multi–core
CPUs)andmanycores(GPUs)architectureshavetobeexploited.
• The memory challenge: Huge datasets cannot be loaded in the memory of a single CPU
and cannot be handled by a single processor but by distributed memory systems. Distributed
computing,basedontheadoptionoftheMPIstandard,representsafeasibleandeffectivesolution.
• Thedatachallenge: Thisaddressesthemanagement,archivingandaccessoftherawdata,the
sciencedataproducts,andthefinaloutcomesofdataprocessingandanalysis.
N-BODYPROBLEM
TheN-bodyproblembroadlydescribestheprob-
| lem of | predicting | the | future | trajectories | of a |     |     |     |
| ------ | ---------- | --- | ------ | ------------ | ---- | --- | --- | --- |
groupofobjectsunderthemutualgravitational
| forces they | exert | on  | one another, | given | each |     |     |     |
| ----------- | ----- | --- | ------------ | ----- | ---- | --- | --- | --- |
individualobject’scurrentpositionandvelocity.
| In Astronomy,                  |              | the N-body | problem      | has             | been   |     |     |     |
| ------------------------------ | ------------ | ---------- | ------------ | --------------- | ------ | --- | --- | --- |
| studiedatawidevarietyofscales: |              |            |              | rangingfrom     |        |     |     |     |
| the study                      | of asteroids |            | near Jupiter | (Brož           | et al. |     |     |     |
| 2008)to                        | the study    | of         | the largest  | gravitationally |        |     |     |     |
| bound clusters                 |              | in the     | Universe     | (Angulo         | et al. |     |     |     |
2012).
| Chapter1. | Introduction |     |     |     |     |     |     | 4   |
| --------- | ------------ | --- | --- | --- | --- | --- | --- | --- |

Includingthecommunicationoverheads,asimpleparallelalgorithmhasthefollowingsteps:
1. BuildtheOctree(onprimaryMPInode),OctreeisbroadcasttoallMPInodes.
2. For each particle, compute the total force by traversing the Octree (particle positions and
velocitiesaredistributedacrossMPInodes).
3. Updatethevelocitiesandpositionsoftheparticles.
4. Repeatsteps2and3foranumberoftimesteps.
5. Outputthefinalpositionsandvelocitiesoftheparticles.
BOOKDefinition: GridComputing
Grid computing is the collection of computer resources from multiple locations to reach
a common goal. The grid can be thought of as a distributed system with non-interactive
workloads that involve a large number of files. Grid computing is distinguished from
conventionalhigh-performancecomputingsystemssuchasclustercomputinginthatgrid
computershaveeachnodesettoperformadifferenttask/application.
Draft
Figure1.6: GridComputing.
UtilitycomputingisatheoreticalconceptusedinCloudComputingtoprovideservicesbasedon
ameteredservicemodel. Thismodelhastheadvantageofalowornoinitialcosttoacquirethe
service; instead, computational resources are essentially rented. The concept isanalogousto other
utilities,likewaterandelectricity,wherethe consumerpaysforwhattheyuse.
• Pay-per-usemodel
• Optimizeresourceutilization
• Outsourcing
• ”Infinite”resources
• Accesstoapplicationsorlibraries
• Automation
Theprincipleofutilitycomputingisverysimple: Onecompanypaysanothercompanyforservicing.
Theservicesincludesoftwarerental,datastoragespace,useofapplicationsoraccesstocomputer
processingpower. Italldependsonwhattheclientwantsandwhatthecompanycanoffer. Different
modelmaybeimplementedevenifthepayperuseisthemostcommonone(e.g. flatrate,metered,
etc). ThepricingmodeliswhatcharacterizetheUtilityComputing.
Chapter1. Introduction 5

EdgeComputingisadistributedcomputingparadigminwhichprocessingandcomputationare
performedmainlyonclassifieddevicenodesknownassmartdevicesoredgedevicesasopposedto
processedinacentralizedcloudenvironmentordatacenters. Ithelpstoprovideserverresources,
dataanalysis,andartificialintelligencetodatacollectionsourcesandcyber-physicalsourceslike
Draft
smartsensorsandactuators. Anetworkofmicrodatacentersembeddedintheinstruments/sensors
that store or process critical data locally and push received data to a centralized data center or
repositoryofcloudstorage. Edge computingprocessesthe datalocallyresultsinreducedtrafficin
thecentralrepository.
• Computingattheedgeofthenetwork
• Focusesonbringingcomputingasclosetothedatasourceaspossible
• Itdecentralizesprocessingpower
• Itreduceslatency
• Itimprovesdatasecurity
• Itreducestheamountofdatathatneedstobemoved
• Itimprovesscalability
Figure1.7: EdgeComputing.
Chapter1. Introduction 6

2
Cloud Computing concept and
architecture
BOOKDefinition: CloudComputing
Cloudcomputing isa modelfor enablingconvenient,on-demand networkaccesstoa shared
poolofconfigurablecomputingresources(e.g.,networks, servers,storage,applications,and
services)thatcanberapidlyprovisionedandreleasedwithminimalmanagementeffortor
serviceproviderinteraction. Thiscloudmodelpromotesavailabilityandiscomposedoffive
essentialcharacteristics,threeservicemodels,andfourdeploymentmodels.
Cloud computing is the on-demand availability of computer system resources, especially data
storage (“cloud storage”) and computing power, without direct active management by the user.
Clouds may be limited to a single organization ( cloud), or be available
private/enterprise
to many organizations ( cloud), maybe a mixture of the two ( cloud); Cloud
public hybrid
implementsa modelbasedontheconceptofinfiniteresourcesavailability;
pay-as-you-go
Baseconcepts:
• Abstraction: the process of removing or rDerdaufcting the complexity of a system by hiding or
suppressingdetails;
• Virtualization: theprocessofcreatingavirtualversionofsomething,includingvirtualcomputer
hardwareplatforms,storagedevices,andcomputernetworkresources;
Figure2.1: CloudComputingArchitecture
2.1 Service Attributes
• On-demandOn-demandcomputingisabusinesscomputingmodelinwhichcomputingresources
are made available to the user on an ”as needed” basis. Rather than all at once, on-demand
computing allows cloud hosting companies to provide their clients with access to computing
Chapter2. CloudComputingconceptandarchitecture 7

resourcesastheybecomenecessary. Theon-demandcomputingmodelovercomesthecommon
challenge that enterprises encountered of not being able to meet unpredictable, fluctuating
computingdemandsefficiently.
• Broad network access Capabilities are available over the network and accessible through
standard mechanisms that promote use by heterogeneous thin or thick client platforms (e.g.,
mobilephones,tablets,laptops,andworkstations).
Figure2.2: Broadnetworkaccess
• ResourcepoolingComputingresourcesareDsrtaofrtage,processing,memory,networkbandwidth
andvirtual machines. Provider’s computingresourcesare pooled to servemultiple consumers,
allocated and deallocated as needed. Tenants are Isolated. Location independence: there is
no control over the exact location of the resources. This has major implications performance,
scalability,security.
Figure2.3: Resourcepooling
• Rapid elasticity Capabilities can be elastically provisioned and released, in some cases
automatically,toscalerapidlyoutwardandinwardcommensuratewithdemand. Totheconsumer,
Chapter2. CloudComputingconceptandarchitecture 8

thecapabilitiesavailableforprovisioningoftenappeartobeunlimitedandcanbeappropriated
inanyquantityatanytime.
• Verticalandhorizontalscaling
– Verticalscaling: addingmoreresourcestoasinglenode
– Horizontal scaling: adding more nodes to a system, such as adding a new computer to a
distributedsoftwareapplication
• Measured service Metering capability of service/resource abstractions in terms of storage,
processing, bandwidth, active user accounts etc. Remember the utility computing and pay as
yougomodel.
2.2 Cloud Service Models
• InfrastructureasaService(IaaS):providesvirtualizedcomputingresourcesovertheinternet.
IaaSisoneofthethreemaincategoriesofcloudcomputingservices,alongsideSoftwareasa
Service(SaaS)andPlatformasaService(PaaS).
• Platform as a Service (PaaS): provides a platform allowing customers to develop, run, and
manage applications without the complexity of building and maintaining the infrastructure
typicallyassociatedwithdevelopingandlaunchinganapp.
• SoftwareasaService(SaaS):isasoftwaredistributionmodelinwhichathird-partyprovider
hostsapplicationsandmakesthemavailabletocustomersoverthe internet.
Draft
Figure2.4: CloudServiceModels
2.3 Cloud Deployment Models
• Public Cloud: It is a cloud computing model where services (such as compute, storage, and
networking) are provided over the internet by a third-party provider and are shared among
multiple customers. These services are typically scalable, pay-as-you-go, and maintained by the
provider.
• Commercialcloud: Referstocloudservicesofferedbyaproviderforbusinessandenterpriseuse.
Itcanincludepublic,private,orhybridcloudsolutions,typicallywithafocusonperformance,
security,compliance,andenterprisesupport.
• Private cloud: A private cloud is a cloud infrastructure that is operated solely for a single
organization. Itmaybemanagedbytheorganizationorathirdpartyandmayexiston-premises
oroff-premises.
• Hybrid cloud: The cloud infrastructure is a composition of two or more distinct cloud
infrastructures(private,community,orpublic)thatremainuniqueentitiesbutareboundtogether
Chapter2. CloudComputingconceptandarchitecture 9

by standardized or proprietary technology that enables data and application portability (e.g.,
cloudburstingforloadbalancingbetweenclouds).
• Communitycloud: Acommunitycloudisonewherethecloudhasbeenorganizedtoservea
commonfunctionorpurpose.
2.4 Cloud Computing Infrastructure
• Data Center: A data center is a facility composed of networked computers and storage that
businessesorotherorganizations useto organize, process, storeand disseminatelarge amounts
ofdata.
• Virtualization: Virtualizationistheprocessofcreatingavirtualversionofsomething,including
virtualcomputerhardwareplatforms,storagedevices,andcomputernetworkresources.
• Hypervisor: Ahypervisor,alsoknownasavirtualmachinemonitor,isaprocessthatcreates
andrunsvirtualmachines(VMs).
• Containerization: Containerizationisalightweightalternativetofullmachinevirtualization
thatinvolvesencapsulatinganapplicationinacontainerwithitsownoperatingenvironment.
• Microservices: Microservicesareasoftwaredevelopmenttechnique—avariantoftheservice-
orientedarchitecture(SOA)architecturalstylethatstructuresanapplicationasacollectionof
looselycoupledservices.
Itisbasedontheconceptofvirtualization,whichallowsforthecreationofmultiplevirtualmachines
onasinglephysicalmachine. Thisallowsforthe efficientuseofresourcesandtheability toscale
up or down as needed. For this, Virtual Machines (VMs) are used, which are software-based
representationsof physical machines. These VMscan be created, modified, and deleted asneeded,
allowingfortheefficientuseofresources. Draft
Figure2.5: VMInfrastructure
Applications such as a Web server or database server that can run on a virtual machine image
are referred to as virtual appliances. Virtual appliances are software installed on virtual servers.
Theyareself-containedandrunonavirtualmachine. Theyarepre-configuredandreadytorun
Chapter2. CloudComputingconceptandarchitecture 10

applications.
2.5 Communication in Cloud Computing
CloudcomputingarisesfromservicesavailableovertheInternetcommunicatingusingthestandard
InternetprotocolsuiteunderpinnedbytheHTTPandHTTPStransferprotocols.
• HTTP:HypertextTransferProtocol(HTTP)isanapplicationprotocolfordistributed,collabora-
tive,hypermediainformationsystems. HTTPisthefoundationofdatacommunicationforthe
WorldWideWeb.
• HTTPS:HypertextTransferProtocolSecure(HTTPS)isanextensionoftheHypertextTransfer
Protocol(HTTP).Itis usedforsecure communicationovera computernetworkand iswidely
usedontheInternet.
• REST:RepresentationalStateTransfer(REST)isasoftwarearchitecturalstylethatdefinesaset
ofconstraintstobeusedforcreatingWebservices.
• SOAP:SimpleObjectAccessProtocol(SOAP)isamessagingprotocolthatallowsprograms
that run on disparate operating systems (such as Windows and Linux) to communicate using
HypertextTransferProtocol(HTTP)anditsExtensibleMarkupLanguage(XML).
EYEObservation: REST
REST is an architectural style that defines a set of constraints to be used for creating web
services. RESTful web services allow the requesting systems to access and manipulate
textualrepresentations ofwebresourcesbyusing auniformand predefinedset ofstateless
operations.
Draft
Figure2.6: REST
AdvantagesofREST:
• Scalability: RESTfulwebservicescanbescaledtoaccommodatealargenumberofclients.
• Performance: RESTfulwebservicesarefasterthanSOAPwebservicesbecausetheyareusually
writteninalightweightlanguagelikeJSON.
• Simplicity: RESTful web services are easier to understand and implement than SOAP web
services.
• Flexibility: RESTful web services can be used with any programming language and can be
Chapter2. CloudComputingconceptandarchitecture 11

easilyintegratedwithotherwebservices.
RESTimplementation:
1. Identifyalltheconceptualentitiesthatwewishtoexposeasservices.
2. CreateaURLtoeachresource.
3. Categorizeourresourcesaccordingtowhetherclientscanjustreceivearepresentationofthe
resourceorwhetherclientscanmodifytheresource.
4. AllresourcesaccessibleviaHTTPGETshouldbeside-effectfree,i.e.,theresourceshould
justreturnarepresentationoftheresource(notmodifyit).
5. Put hyperlinks in the representation of the resource to allow clients to navigate to related
resources.
6. Designtorevealdatagradually. Don’trevealanythinginasingleresponsedocument. Provide
hyperlinkstoobtainmodedetails.
7. Specifythe format ofresponse datausinga schema (DTD,W3C Schema, RelaxNG,...). For
thoseservices thatrequire aPOST orPUT toit, alsoprovide aschemato specifythe format
oftheresponse.
8. DescribehowourservicesaretobeinvokedusingeitheraWSDLdocumentorsimplyan
HTMLdocument.
Draft
Chapter2. CloudComputingconceptandarchitecture 12

3
|     |     |     | Computing |     |     | basics |     |
| --- | --- | --- | --------- | --- | --- | ------ | --- |
Whatisacomputer?
Acomputerisamachinethatcanbeprogrammedtoautomaticallycarryoutsequencesofarithmetic
orlogicaloperations (computation). Moderndigitalelectroniccomputerscan performgeneric sets
ofoperationsknownasprograms. Theseprogramsenablecomputerstoperformawiderangeof
tasks. The term computer system may refer to a nominally complete computer that includes the
hardware,operatingsystem,software,andperipheralequipmentneededandusedforfulloperation;
AComputerclusterisagroupofcomputersthatarelinkedandfunctiontogether.
|     |     |     | A serial                         | computer      | is a computer | that pro-  |     |
| --- | --- | --- | -------------------------------- | ------------- | ------------- | ---------- | --- |
|     |     |     | cesses                           | data one bit  | at a time.    | This is in |     |
|     |     |     | contrast                         | to a parallel | computer,     | which pro- |     |
|     |     |     | cessesmultiplebitsatthesametime. |               |               | Serial     |     |
|     |     |     | computers                        | are much      | slower than   | parallel   |     |
|     |     |     | computers,                       | but they      | are also much | simpler    |     |
andcheapertobuild.
Draft
|     | Figure3.1: SerialComputer |     |     |     |     |     |     |
| --- | ------------------------- | --- | --- | --- | --- | --- | --- |
Today,themostcommonformofcomputerisadigitalcomputer,andcomputingmachineshave
beenanintegralpartofthebusinessandengineeringworldsincethelate20thcentury. Computers
are used in a wide range of applications, including data processing, business management, and
wordprocessing. Theyarealsousedinscientific research,engineering,andmedicine. Computers
areusedtocontrolindustrialprocessesandtosimulatecomplexsystems. Moderncomputersare
capableofperformingawiderangeoftasks,fromsimplearithmetictocomplexcalculations.
| A parallel | computer | is a computer | that |     |     |     |     |
| ---------- | -------- | ------------- | ---- | --- | --- | --- | --- |
processesdatamultiplebitsatthesametime.
Thisisincontrasttoaserialcomputer,which
| processesdataonebitatatime. |     | Parallelcom- |     |     |     |     |     |
| --------------------------- | --- | ------------ | --- | --- | --- | --- | --- |
putersaremuchfasterthanserialcomputers,
| but they | are also much | more complex | and |     |     |     |     |
| -------- | ------------- | ------------ | --- | --- | --- | --- | --- |
expensivetobuild.
Figure3.2: ParallelComputer
| Chapter3. | Computingbasics |     |     |     |     |     | 13  |
| --------- | --------------- | --- | --- | --- | --- | --- | --- |

There are two main types of parallel computers: shared memory and distributed memory. In a
sharedmemorysystem, allprocessorsshare thesamememory. In adistributedmemory system,
eachprocessorhasitsownmemory. Parallelcomputersareusedinawiderangeofapplications,
includingscientificresearch,engineering,andbusiness.
• Sharedmemory: Inasharedmemorysystem,allprocessorssharethesamememory. Thisallows
processorstocommunicatewitheachotherbyreadingandwritingtothesamememorylocations.
Sharedmemorysystemsaretypicallyusedinapplicationsthatrequirehighperformanceandlow
latency,suchasscientificresearchandengineering.
– UniformMemoryAccess(UMA):eachprocessorhasuniformaccesstomemory.
Figure3.3: UniformMemoryAccess(UMA)
– Non-UniformMemoryAccess(NUMA):timeformemoryaccessdependsonlocationofdata.
Draft
Localaccessisfasterthannon-localaccess.
Figure3.4: Non-UniformMemoryAccess(NUMA)
• Distributed memory: In a distributed memory system, each processor has its own memory.
This allows processors to communicate with each other by sending messages over a network.
Distributedmemory systemsare typically usedin applications thatrequire scalability andfault
tolerance,suchaslarge-scaledataprocessingandcloudcomputing.
Figure3.5: DistributedMemory
Chapter3. Computingbasics 14

3.1 Cluster
Acomputerclusterisagroupofcomputersthatarelinkedandfunctiontogether. Thecomponents
ofaclusterareusuallyconnectedtoeachotherthroughfastlocalareanetworks,witheachnode
runningitsowninstanceofanoperatingsystem. Clustersareusedforawiderangeofapplications,
including scientific research, engineering, and business. They are also used in cloud computing
andhigh-performancecomputing.
Figure3.6: ComputerCluster
Compopnentsofacluster:
• Nodes: Anodeisasinglecomputerinacluster. Eachnodehasitsownprocessor,memory,and
storage. Nodesareconnectedtoeachotherthroughanetwork.
• Network: Thenetworkisthecommunicationinfrastructurethatconnectsthenodesinacluster.
Thenetworkallowsnodestocommunicatewitheachotherandshare data.
Draft
• OperatingSystem: Eachnodeinaclusterrunsitsowninstanceofanoperatingsystem. The
operating system manages the resources of the node and provides an interface for running
applications.
• Applications: Applicationsareprogramsthatrunonthenodesinacluster. Applicationscanbe
parallelizedtotakeadvantageoftheresourcesofthecluster.
Figure3.7: ComponentsofaCluster
Whatdoesanodecontain?
Figure3.8: NodeComponents
Chapter3. Computingbasics 15

CPU are multicore proccessors. Because
of power, heat dissipation, ..., increasing
tendencytoactuallylowerclockfrequency
butpackmorecomputingcoresontoachip.
These cores will share some resources, i.e.
memory, network, disk, etc. but are still
capableofindependentcalculations.
Figure3.9: multicoreCPU
BOOKDefinition: Core
Acoreisthesmallestunitofcomputing,havingoneormore(hardware/software)threads
andisresponsibleforexecutinginstructions.
Intel®Hyper-ThreadingTechnologyusesprocessorresourcesmoreefficiently,enablingmultiple
threadstorunoneachcore. TheOS”seestwocoresandtransparentlytrytoexecutetwoprograms
ontwodifferent”cores”.
Abitofjargon:
• Multiprocessor=serverwithoneormorethan1CPU
• Multicore=CPUwithmorethan1core
Draft
• Processor=CPU=socket=chip
Figure3.10: ComputingCluster
Figure3.11: HPCcluster
Theschedulerisasoftwarecomponentthatmanagestheresourcesofacluster. Itisresponsible
for allocating resources to applications and ensuring that they run efficiently. The scheduler is
typicallypartoftheoperatingsystemoftheclusterandisresponsibleformanagingtheresourcesof
thecluster,suchasCPU,memory,andstorage. Theschedulerusesalgorithmstoallocateresources
toapplicationsbasedontheirrequirementsandpriorities.
• Itallocatesexclusiveornon-exclusiveaccesstotheresouces(thenodes)tousersduringalimited
amoutoftimesothattheycanperformtheirwork
• Itabitratescontentionforresourcesbymanaginaaqueueofpendingwork
• Itpermitstoschedulejobsforusersontheclusterresources
Auserjobischaracterizedby:
Chapter3. Computingbasics 16

• thenumberofnodes
• thenumberofCPUcores
• thememoryrequested
• thewalltime
• thelauncherscript,whichwillinitiateyourtask
Apartitionisagroupofcomputenodes,withspecificusagecharacteristics,liketimelimitsand
maximumnumberofnodesperjob.
Draft
Chapter3. Computingbasics 17

4
Virtualization
Virtualizationusessoftwaretocreateanabstractionlayer overcomputerhardwarethatallowsthe
hardwareelementsofa singlecomputer-processors,memory,storageandmore -tobedivided
intomultiplevirtualcomputers,commonlycalledvirtualmachines(VMs). EachVMrunsitsown
operatingsystem(OS)andbehaveslikeanindependentcomputer,eventhoughitisrunningonjust
aportionoftheactualunderlyingcomputerhardware.
Draft
Figure4.1: Virtualization
It is the ability to “simulate” a hardware platform, such as a server, storage device or network
resource, in software. All the functionality is separated (abstracted) from the hardware and
“simulated” as a “virtual instance” with the ability to operate just like the hardware solution.
Asinglehardwareplatformcanbeusedtosupportmultiplevirtualdevicesormachines,whichare
easytospinupordownasneeded.
VirtualMachinereferstoasoftwaresimulationofacomputer. ItcanrunanOSandapplications
interacting with the virtualized abstracted resources, not with the physical resourcesof the actual
hostcomputer.
Hypervisor (or VM monitor) refers to a software tool installed on the physical host system to
provide the this software layer of abstraction that decouples the OS from the physical bare-metal.
It allows to split a computer in different separate environments, the VMs, distributing them the
computerresources.
Components:
• HostOS:theOSrunningonthephysicalmachine.
• Hypervisor: thesoftwarelayerthatabstractsthehardwareandcreatestheVMs.
• GuestOS:theOSrunningontheVM.
Chapter4. Virtualization 18

Figure4.2: Virtualizationcomponents
| 4.1 | Virtual Machines |     |
| --- | ---------------- | --- |
AVirtualMachineisavirtualcomputingsystem. Ithastighly isolatedsoftwarewithanOSand
| applicationsinside. | EachVMinahostisindependent. |     |
| ------------------- | --------------------------- | --- |
In a single physical server can be put multiple VMs enabling the run of multiple OSes and
Applications(partition/multi-tenancy).
Features:
| • Consolidation: | multipleVMsonasinglephysicalserver. |     |
| ---------------- | ----------------------------------- | --- |
| • Isolation:     | VMsareindependent.                  |     |
| • Encapsulation: | VMsareportable.                     |     |
• HardwareIndependence: VMsarenottiedtothephysicalhardware.
| • Security: | VMsareisolatedfromeachother. |     |
| ----------- | ---------------------------- | --- |
Draft
4.2 Virtualization models
Figure4.3: Virtualizationmodels
1. Type 1 Hypervisor: it runs directly on the host’s hardware to control the hardware and
to manage guest operating systems. It is also called bare metal hypervisor. It is the most
efficientbecauseithasdirectaccesstothehardware. Examples: VMwareESXi,Microsoft
Hyper-V,CitrixXenServer,OracleVMServerforSPARC.
2. Type 2 Hypervisor: it runs on a conventional operating system just as other computer
programs do. It is also called hosted hypervisor. It is easier to set up and use, but it is
lessefficientbecauseitmustusethehost OStoaccessthehardware. Examples: VMware
Workstation,VMwarePlayer,OracleVirtualBox,ParallelsDesktopforMac.
| Chapter4. | Virtualization | 19  |
| --------- | -------------- | --- |

Anotherdistinctioncanbemadew.r.t. virtualization:
• Fullvirtualization: thehypervisorprovidescompletehardwareabstractioncreatingsimulated
hardware devices. The guest OS don’t know (or care) about the presence of a hypervisor and
issuecommandstowhatitthinksisactualhardware.
• Paravirtualization: theguestOSisawareofthehypervisorandinteractswithit
• Hardware-assistedvirtualization: thehypervisoruseshardwarecapabilitiesto
Exclamation-TriangleWarning: HardwareProtectionLevels
Sincecomputersrunmorethanonsoftwareprocess,thiswillbringsomeissues. Protection
ringsareoneofthekeysolutionsforsharingresourcesandhardware.
• Ring0: themostprivilegedlevel(kernelmode).
• Ring1-2-3: lessprivilegedlevels(usermode).
ThehypervisorrunsinRing0,theguestOSrunsinRing1-2-3.
4.3 Virtualizing components
4.3.1 CPU
The hypervisor must manage the CPU resources. It must decide which VM gets CPU time and
when. ItmustalsomanagetheCPUinstructionsthatareexecutedbytheVMs.
Guest instructions are executed directly by the hardware as much as possible: virtual machine
monitordoesnotinterferewitheverysingleinstructionthatisissuedbytheguestoperatingsystem,
oritsapplications. Thenon-privilegedinstructionswilloperateathardwarespeeds.
Draft
Privilegedinstructions: Wheneveraprivilegedinstructiongetsaccessed,thentheprocessorcauses
atrap,andcontrolisautomaticallyswitchedtothemostprivilegedlevel,thatisthehypervisor. At
thispoint,thehypervisorcandeterminewhethertheoperationistobeanallowedornot. Illegal
operationswillcauseactionsontheVM(asKILL),legaloperationsthehypervisorshouldperform
thenecessaryemulationsothattheguestoperatingsystemisundertheimpressionthatitdoeshave
controloverthehardware.
4.3.2 Memory
Fullvirtualization: theguestoperatingsystemcontinuestoobserve acontiguouslinearphysical
addressspacethatstartsfromphysicaladdress0. Threetypesofaddresses:
• Virtualaddresses,thesearetheonesthatareusedbytheapplicationsintheguest
• Physical addresses, these are the ones that the guest thinks are the addresses of the physical
resources
• Themachineaddresses,thesearetheactualmachineaddresseswiththeactualphysicaladdresses
ontheunderlyingplatform.
Chapter4. Virtualization 20

Figure4.4: Memoryvirtualization
Everysinglememoryaccessgoesthroughtwoseparatetranslation,thefirstonewhichwillbedone
insoftware,andthenthesecondonepotentiallycantakeadvantageofhardware.
Moreover,ShadowPageTable estabilishesashortcuttodirectlymanagethemappingfrom GVA
to HPA. It works the same way virtual addresses is mapped to physical addresses. VMM must
maintainconsistencebetweenthepagetables.
Draft
Figure4.5: ShadowPageTable
Paravirtualization allowsguest operatingsystems tobeawareoftheirvirtual environmentand
communicate directly with the hypervisor to manage memory. Instead of fully abstracting the
underlyinghardware,theguestOSusesanoptimizedinterfacetoreducetheoverheadoftrapping
into privileged operations. This approach decreases the complexity of mapping guest physical
addressestomachine addressesby enablingthe hypervisorandguest OStocollaborateonpage
tablemanagementandmemoryaccess,leadingtomore efficientperformanceoverall.
4.3.3 Network
The hypervisor must manage the network resources. It must decide which VM gets network
accessandwhen. Itmustalsomanagethe networkpackets thatare sentand receivedby theVMs.
Applicationsrunonavirtualnetworkastheywhererunningonaphysicalnetwork.
Chapter4. Virtualization 21

• Flexibility: VMs can be moved between physical servers without changing the network
configuration.
| • Manageability: | VMscanbemanagedasasingleentity. |     |
| ---------------- | ------------------------------- | --- |
| • Scalability:   | VMscanbeaddedorremovedasneeded. |     |
| • Security:      | VMsareisolatedfromeachother.    |     |
• Programmability: VMscanbecontrolledbysoftware.
| • Heterogeneity: | VMscanrundifferentOSesandapplications. |     |
| ---------------- | -------------------------------------- | --- |
Draft
Figure4.6: Networkvirtualization
| 4.3.4 | Disk |     |
| ----- | ---- | --- |
The hypervisor must manage the disk resources. It must decide which VM gets disk access and
when. ItmustalsomanagethediskblocksthatarereadandwrittenbytheVMs. Applicationsrun
onavirtualdiskastheywhererunningonaphysicaldisk. VirtualDisksarewhereguestoperating
systemsareinstalled,makingthemtheequivalentoftraditionalharddisks. Theyarestoredasfiles
onthehostsystem’sphysicaldisk.
• Flexibility: VMscanbemovedbetweenphysicalserverswithoutchangingthediskconfiguration.
| • Manageability: | VMscanbemanagedasasingleentity. |     |
| ---------------- | ------------------------------- | --- |
| • Scalability:   | VMscanbeaddedorremovedasneeded. |     |
| • Security:      | VMsareisolatedfromeachother.    |     |
• Programmability: VMscanbecontrolledbysoftware.
| • Heterogeneity: | VMscanrundifferentOSesandapplications. |     |
| ---------------- | -------------------------------------- | --- |
| Chapter4.        | Virtualization                         | 22  |

4.4 Emulation
Emulationisthe processofsimulatinghardwareusing software. It isusedwhen theguest OSis
notawareofthehypervisor. ThehypervisormustemulatethehardwarethattheguestOSexpects
tosee.
Emulation brings higher overhead but has its perks too. It is highly inexpensive, easy to access,
and helps us run the programs that have become obsolete in the available system. Anyone can
accesstheemulationplatformsremotelyandiseasiertouse. Itisanexcellentabilitytohavefor
embedded/OSdevelopment,withoutaffectingtheunderlyingOS.
Draft
Chapter4. Virtualization 23

5
Benchmarking a Linux platform
Benchmarkingistheprocessofmeasuringtheperformanceofasystem. Itisacriticalstepinthe
processofdesigninganddeployingasystem. Itisimportantfor:
• Maximize efficiency: understanding how the server utilizes the CPU, memory, storage and
networkresourcesallowsforfine-tuningandmakingbetteruseofthehardwarrefull potential.
• Costoptimization: understandingtheperformanceofthesystemallowstomakebetterdecisions
onthehardwaretobuy.
• Troubleshootingissues: benchmarkingcanhelptoidentifybottlenecksandperformanceissues.
• Security: benchmarkingcanhelptoidentifysecurityissues.
• Comparingsystems: benchmarkingcanhelptocompare differentsystems.
5.1 Components to test
Therearea lot of toolsavailableon the Internet that can perform systemtesting on Linux. Wecan
usethemtorundifferenttestsandbenchmarkvariouscomponents,suchastheCentralProcessing
Unit(CPU),theGraphicsProcessingUnit(GPU),memory,database,andmore.
Draft
Figure5.1: Componentstotest
• CPU: the CPU handles all computational tasks. Measuring its performances helps in under-
standing how efficiently it handles processes and threads, or whether there are bottlenecks in
processingpower.
– stress-ng
– sysbench
– htop
– mpstat
– HPL
– HPCG
– SPEC
Chapter5. BenchmarkingaLinuxplatform 24

• Memory (RAM): the memory is used to store data and instructions that are currently in use.
Measuringitsperformanceshelpsinunderstandinghowefficientlyithandlesdataandinstructions,
orwhethertherearebottlenecksinmemoryusage.
– stress-ng
– memtester
– memtest86
– valgrind
– cachegrind
– callgrind
• Dist I/O and Storage: the storage is used to store data and instructions that are not currently
in use. Measuring its performances helps in understanding how efficiently it handles data and
instructions,orwhethertherearebottlenecksinstorageusage.
– fio
– ioping
– dd
– hdparm
– bonnie++
– iozone
• Network: the network is used to transfer data and instructions between different systems.
Measuringitsperformanceshelpsinunderstandinghowefficientlyithandlesdataandinstructions,
orwhethertherearebottlenecksinnetworkusage.
– iperf
Draft
– netperf
– nttcp
– ping
– traceroute
– mtr
5.2 Benchmarking tools
5.2.1 HP Linpack (HPL)
ThisisthemostfamousHPCbenchmark,usedforthe”Top500”ranking. Itisasoftwarepackage
thatsolvesarandomsystemoflinearequationsusingGaussianelimination(matrixisdense).
For a given problem size N, HPL performs floating point operations proportional to N*N*N
operations while performing memory reads and write proportional to N*N. That means, if the
problemsizedoubles,thenumberoffloating-pointoperationsincreasesbyafactorof8whilethe
number of memory operations only grows by a factor of 4. This property of HPL means that it
performs very well on systems that have many flop functional units relative to data movement
support. TheresultisthatHPLtendstorepresentaveryoptimisticperformancepredictionthatcan
onlybematchedbyasmallnumberofrealscientificapplications.
5.2.2 High Performance Conjugate Gradient (HPCG)
HPCG uses a simple implementation of the Conjugate Gradients and multigrid algorithms. It
generatesandusessparsedatastructuresthathaveaverylowcompute-to-data-movementratio.
Chapter5. BenchmarkingaLinuxplatform 25

HPCG has a floating-point operations rate proportional to N and a memory access rate also
proportionaltoN.ThispropertymeansthatHPCGperformanceisstronglyinfluencedbymemory
bandwidth.
5.3 Tests
| 5.3.1 | CPU |     |     |     |     |
| ----- | --- | --- | --- | --- | --- |
TheCPU is responsible for executing all the processes, calculations, and tasks. A server’s overall
speed, responsiveness, and ability to handle concurrent workloads are highly dependent on the
efficiencyandpowerofitsCPU.
Inavirtualizedenvironment,CPUtestingbecomesevenmorecriticalsincephysicalCPUresources
aresharedbetweenthevirtualmachines(VMs)runningonthehost. ThevirtualCPUs(vCPUs)
allocatedtoeachVMmustbecarefullymonitoredtoensurethatthehostandallguestVMsoperate
efficientlywithoutresourcecontention.
Totestit,weconsidertheFloatingPointOperationsPerSecond(FLOPS):
|     |     | Number | of floating point | operations |     |
| --- | --- | ------ | ----------------- | ---------- | --- |
FLOPS=
Time
FloatingpointoperationsperclockcyclepercoreisCPUdependent.
WhentestingCPUperformance,severalkeymetricsprovideinsightintohowwelltheCPUhandles
thesystem’swordload.
• CPUutilization: thepercentageoftimetheCPUisbusyprocessinginstructions.
| • CPUload:        | thenumberofprocessesintherunqueue. |     |     |     |     |
| ----------------- | ---------------------------------- | --- | --- | --- | --- |
| • CPUtemperature: | thetemperatureoftheCDPraUft.       |     |     |     |     |
• CPUfrequency: thefrequencyoftheCPU.
| • CPUcache: | thecacheoftheCPU. |     |     |     |     |
| ----------- | ----------------- | --- | --- | --- | --- |
• CPUWaitTime: thetimetheCPUiswaitingforI/Ooperationstocomplete.
SometoolstotesttheCPUare:
1. Baselinetests: Start by measuringCPU performance under typical workloads to establish a
baseline. This allows you to understand how the CPU performs under normal conditions.
| Useasexample |     | (seeHPLCalculator). |     |     |     |
| ------------ | --- | ------------------- | --- | --- | --- |
HPL
2. Stresstests: PushtheCPUtoitslimitstoseehowitperformsunderheavyworkloads. Use
| asexample |           | or       | . ItcancreatestressonCPU,cacheandmemory. |     |     |
| --------- | --------- | -------- | ---------------------------------------- | --- | --- |
|           | stress-ng | sysbench |                                          |     |     |
3. htop: Aninteractivesystemmonitorthatprovidesreal-timeinformationonCPUutilization,
loadaverage,andthenumberofrunningprocesses. Itvisuallyrepresentsper-coreutilization.
| Simplyrun | fromthecommandline. |     |     |     |     |
| --------- | ------------------- | --- | --- | --- | --- |
htop
4. mpstat: Acommand-lineutilitythatprovidesreal-timeCPUstatistics. Itdisplaysinformation
onCPUutilization,idletime,andwaittime. Run fromthecommandline.
mpstat
Analysisontheresults:
• CPUutilization: thepercentageoftimetheCPUisbusyprocessinginstructions. AhighCPU
utilizationindicatesthattheCPUisworkinghardtoprocessinstructions. AlowCPUutilization
indicatesthattheCPUisnotworkinghardtoprocessinstructions.
• CPUload: thenumberofprocessesintherunqueue. AhighCPUloadindicatesthatthereare
manyprocesseswaitingtobeexecuted. AlowCPUloadindicatesthatthereare fewprocesses
waitingtobeexecuted.
• HighContextSwitches: AhighnumberofcontextswitchesindicatesthattheCPUisswitching
| Chapter5. | BenchmarkingaLinuxplatform |     |     |     | 26  |
| --------- | -------------------------- | --- | --- | --- | --- |

| betweenprocessesfrequently. |     |     | Thiscanleadtoperformanceissues. |     |     |
| --------------------------- | --- | --- | ------------------------------- | --- | --- |
• HighInterrupts: AhighnumberofinterruptsindicatesthattheCPUishandlingalargenumber
| ofhardwareinterrupts. |     | Thiscanleadtoperformanceissues. |     |     |     |
| --------------------- | --- | ------------------------------- | --- | --- | --- |
• vCPUOvercommitment: OvercommittingvCPUscanleadtoperformanceissues. Makesure
thatthe numberof vCPUsallocated toa VMdoes notexceedthe numberof physicalcores on
thehost.
| 5.3.2 | Memory |     |     |     |     |
| ----- | ------ | --- | --- | --- | --- |
Memoryisusedtostoredataandinstructionsthatarecurrentlyinuse. Measuringitsperformance
helps in understanding how efficiently it handles data and instructions, or whether there are
bottlenecksinmemoryusage.
Memoryisoneofthemostvitalcomponentsofaserver,affectinghowquicklyapplicationscan
accessandmanipulatedata. Inadequatememoryresources,poormemorymanagement,orissues
withmemoryspeedcanleadtosevereperformancebottlenecks. Forexample,ifthesystemrunsout
ofphysicalmemory,itmaystartusingswapspace(disk-basedmemory),whichcansignificantly
degradeperformance.
Testing memory performance ensures that the system can handle workloads effectively without
encountering delays due to insufficient or slow memory. This is particularly important in
environmentsrunningdatabases,virtualmachines,orin-memorydatastores(likeRedis).
Whentesting memory performance, several key metrics provide insight intohow well the memory
handlesthesystem’sworkload.
| • Memoryutilization: |     | thepercentageofmemorythat               |     | isbeingused. |     |
| -------------------- | --- | --------------------------------------- | --- | ------------ | --- |
| • Memoryload:        |     | thenumberofprocessesthatareusingmemory. |     |              |     |
Draft
| • Memorylatency: |     | thetimeittakestoaccessmemory. |     |     |     |
| ---------------- | --- | ----------------------------- | --- | --- | --- |
• Memorybandwidth: therateatwhichdatacanbereadfromorwrittentomemory.
| • Memoryerrors:       |     | thenumberofmemoryerrorsthatoccur. |     |         |     |
| --------------------- | --- | --------------------------------- | --- | ------- | --- |
| • CacheHit/MissRatio: |     | theratioofcachehitstocache        |     | misses. |     |
Sometoolstotestthememoryare:
1. Baselinetests: Startbymeasuringmemoryperformanceundertypicalworkloadstoestablish
abaseline. Thisallowsyoutounderstandhowthememoryperformsundernormalconditions.
| Useasexample |     |           | or        | .   |     |
| ------------ | --- | --------- | --------- | --- | --- |
|              |     | memtester | memtest86 |     |     |
2. Stresstests: Pushthememorytoitslimitstoseehowitperformsunderheavyworkloads.
| Useasexample |     |           | or        | .   |     |
| ------------ | --- | --------- | --------- | --- | --- |
|              |     | stress-ng | memtester |     |     |
3. valgrind: Amemorydebugging toolthat canbe usedto profilememory usageand detect
| memoryleaks. |     | Run | fromthecommandline. |     |     |
| ------------ | --- | --- | ------------------- | --- | --- |
valgrind
4. cachegrind: Acacheprofilingtoolthatcanbeusedtoprofilecacheusage. Run
cachegrind
fromthecommandline.
5. callgrind: A call graph profiling tool that can be used to profile function calls. Run
fromthecommandline.
callgrind
Analysisontheresults:
• Memoryutilization: thepercentageofmemorythatisbeingused. Ahigh memoryutilization
indicates that the memory is being used heavily. A low memory utilization indicates that the
memoryisnotbeingusedheavily.
• Memoryload: thenumberofprocessesthatareusingmemory. Ahighmemoryloadindicates
thattherearemany processesusingmemory. Alowmemoryloadindicatesthatthereare few
| Chapter5. | BenchmarkingaLinuxplatform |     |     |     | 27  |
| --------- | -------------------------- | --- | --- | --- | --- |

processesusingmemory.
• Memory latency: the time it takes to access memory. A low memory latency indicates that
memoryaccessisfast. Ahighmemorylatencyindicatesthatmemoryaccessisslow.
• Memory bandwidth: the rate at which data can be read from or written to memory. A high
memorybandwidthindicatesthatmemoryaccessisfast. Alowmemorybandwidthindicates
thatmemoryaccessisslow.
• Memory errors: the number of memory errors that occur. A high number of memory errors
indicatesthatthereareissueswithmemory.
• Cache Hit/Miss Ratio: the ratio of cache hits to cache misses. A high cache hit/miss ratio
indicatesthatthe cacheisbeing usedeffectively. Alowcachehit/missratioindicatesthatthe
cacheisnotbeingusedeffectively.
5.3.3 Disk I/O and Storage
Disk I/O (Input/Output) is a critical factor in server performance, especially for databases, web
servers, and applications that rely on frequent disk access. Slow disk performance can lead to
bottleneckswhereapplicationsarewaitingondataretrievalorwritingoperations,causingdelays
andsystemslowdowns. Inaddition,storagesubsystemslikeRAIDarraysandSSDshavedifferent
performancecharacteristicsthatmustbeoptimizedfortheserver’sworkload.
CloudandVirtualizedenvironmentsalsointroduceadditionalcomplexitysincemultiplevirtual
machinesmaysharethesamephysicalstorage,increasingthepotentialforresourcecontention.
WhentestingdiskI/Oandstorageperformance,severalkeymetricsprovideinsightintohowwell
thestoragesubsystemhandlesthesystem’sworkload.
• Read/WriteThroughput: therateatwhichdatacanbereadfromorwrittentodisk.
Draft
• Read/WriteLatency: thetimeittakestoreadfromorwritetodisk.
• IOPS(Input/OutputOperationsPerSecond): thenumberofread/writeoperationsthatcanbe
performedpersecond.
• DiskQueueLength: thenumberofread/writerequeststhat arewaitingtobeprocessed.
• DiskErrors: thenumberofdiskerrorsthatoccur.
• SSDvsHDD:theperformancedifferencebetweenSSDsandHDDs.
SometoolstotestthediskI/Oandstorageare:
1. Baselinetests: StartbymeasuringdiskI/Oandstorageperformanceundertypicalworkloads
toestablish a baseline. This allowsyou to understand howthe storage subsystem performs
undernormalconditions. Useasexample or .
fio dd
2. Stresstests: PushthediskI/Oandstoragetoitslimitstoseehowitperformsunderheavy
workloads. Useasexample or .
fio dd
3. ioping: Adisk I/O latencytoolthat can beused to measuredisk I/Olatency. Run
ioping
fromthecommandline.
4. hdparm: A disk performance tool that can be used to measure disk performance. Run
fromthecommandline.
hdparm
5. bonnie++: Adiskbenchmarkingtoolthatcanbeusedtomeasurediskperformance. Run
fromthecommandline.
bonnie++@
6. iozone: A disk benchmarking tool that can be used to measure disk performance. Run
fromthecommandline.
iozone
Analysisontheresults:
• Read/Write Throughput: the rate at which data can be read from or written to disk. A high
Chapter5. BenchmarkingaLinuxplatform 28

read/writethroughputindicatesthatthediskI/Oandstoragesubsystemisperformingwell. A
lowread/writethroughputindicatesthatthediskI/Oandstoragesubsystemisnotperforming
well.
• Read/WriteLatency: thetimeittakestoreadfromorwritetodisk. Alowread/writelatency
indicatesthat diskI/Oand storageaccess isfast. Ahigh read/writelatencyindicates thatdisk
I/Oandstorageaccessisslow.
• IOPS(Input/OutputOperationsPerSecond): thenumberofread/writeoperationsthatcan
be performed per second. A high IOPS indicates that the disk I/O and storage subsystem is
performingwell. AlowIOPSindicatesthatthediskI/Oandstoragesubsystemisnotperforming
well.
• DiskQueueLength: thenumberofread/writerequeststhatarewaitingtobeprocessed. Ahigh
diskqueuelengthindicatesthattherearemanyread/writerequestswaitingtobeprocessed. A
lowdiskqueuelengthindicatesthattherearefewread/writerequestswaitingtobeprocessed.
• DiskErrors: thenumberofdiskerrorsthatoccur. Ahighnumberofdiskerrorsindicatesthat
thereareissueswiththediskI/Oandstoragesubsystem.
• SSDvsHDD:theperformancedifferencebetweenSSDsandHDDs. SSDsarefasterthanHDDs
andhavelowerlatency.
Draft
Chapter5. BenchmarkingaLinuxplatform 29

6
Containers
| Virtual | Environments         |                                      |     |     |     |
| ------- | -------------------- | ------------------------------------ | --- | --- | --- |
| Pros    |                      | Cons                                 |     |     |     |
| •       | Reproducibleresearch | • Difficultysettingupyourenvironment |     |     |     |
| •       | Explicitdependencies | • Notisolation                       |     |     |     |
• Doesnotalwaysworkacrossdifferent
• Improvedengineeringcollaboration
OS
| Virtual | Machines |      |     |     |     |
| ------- | -------- | ---- | --- | --- | --- |
| Pros    |          | Cons |     |     |     |
• Fullautonomy
• Useshardwareinlocalmachine
• Verysecure
• NotveryportablesincesizeofVMsare
• Isolation
large
| •   | Lowercosts Draft |         |                |            |      |
| --- | ---------------- | ------- | -------------- | ---------- | ---- |
|     |                  | • There | is an overhead | associated | with |
• usedbyallCloudproviders
virtualmachines
Exclamation-TriangleWarning: DependencyHell
• Dependency Hell is a colloquial term for the frustration of some software users who
haveinstalledsoftware packages which havedependencieson specific versionsof other
softwarepackages.
• Itisaproblemwithsoftwaredependenciesthatoccurswhenasoftwarepackageisdesigned
toworkwithparticularversionsofothersoftwarepackages.
• Ifa userwantsto installa softwarepackagethat hasdependencies onspecific versionsof
othersoftwarepackages,itcanbedifficulttoresolvethese dependencies.
ThesolutionforalltheseproblemsisusingContainers.
BOOKDefinition: Container
Informally, a container is a standard unit of software that packages up code and all its
dependenciesinprocessesisolatedfromresources,sotheapplicationrunsquicklyandreliably
from one computing environment to another. Containers creates an isolated environment at
applicationlevelandnotatserverlevel.
| Chapter6. | Containers |     |     |     | 30  |
| --------- | ---------- | --- | --- | --- | --- |

Theycomewithdifferentadvantages:
• Extremelyportableandlightweight
• Fullypackagedsoftwarewithalldependenciesincluded
• Canbeusedfordevelopment,trainingandanddeployment
• Developmentteamscaneasilysharecontainers
A container image is a lightweight, stand-alone, executable package of a piece of software
that includes everything needed to run it: code, runtime, system tools, system libraries, settings.
Available for both Linux and Windows based apps, containerized software will always run the
same,regardlessoftheenvironment.
Container images become containers at runtime and in the case of Docker containers - images
becomecontainerswhentheyrunonDockerEngine. AvailableforbothLinuxandWindows-based
applications,containerizedsoftwarewillalwaysrunthesame,regardlessoftheinfrastructure.
Draft
Figure6.1: ComparisonbetweenVenvs,VMsandContainers
6.1 Properties
• Isolation: Containers virtualize CPU, memory, storage, and network resources at the OS-level,
providingdeveloperswithasandboxedviewoftheOSlogicallyisolatedfromotherapplications.
Developers, using containers, are able to create predictable environments isolated from other
applications.
• Productivity: enhancement Containers can include software dependencies needed by the
application(specificversionsofprogramminglanguageruntimes,softwarelibraries)guaranteed
tobeconsistentnomatterwheretheapplicationisdeployed. Allthistranslatestoproductivity:
developers and IT operations teams spend less time debugging and diagnosing differences in
environments,andmoretimeshippingnewfunctionalityforusers.
• Deploymentsimplicity: Containerscanbedeployedonanymachinethathasacontainerruntime
installed. Thismeansthatdeveloperscanbuildandtestcontainersontheirlocalmachineand
then deploy them to any environment that supports containers. This makes it easy to move
applicationsbetweenenvironments,suchasfromadeveloper’slaptoptoatestenvironment,or
fromatestenvironmenttoproduction.
• Portability: Containersareportable becausetheyinclude allofthe dependenciesneededto run
theapplication. Thismeansthatcontainerscanrunonanymachinethathasacontainerruntime
installed,regardless ofthe underlyinginfrastructure. Thismakesiteasytomove applications
betweenenvironments,suchasfromadeveloper’slaptoptoatestenvironment,orfromatest
environmenttoproduction.
Chapter6. Containers 31

• Easy Versioning: Containers make it easy to version applications. Developers can create a
new version of an application by creating a new container image with the new version of the
application. Thismakesiteasytorollbacktoapreviousversionofanapplicationifthereare
anyissueswiththenewversion.
• Scalability: Containers are lightweight and can be started and stopped quickly. This makes
it easy to scale applications up and down based on demand. For example, if an application is
experiencinghightraffic,additionalcontainerscanbestartedtohandletheload. Oncethetraffic
decreases,theadditionalcontainerscanbestopped.
• Operationalefficiencyand reliability: Containers make iteasyto automate the deployment
andmanagementofapplications. Thismakesit easytodeployapplicationsquicklyandreliably.
Containerscanalsobeusedtoautomatethescalingofapplicationsbasedondemand. Thismakes
iteasytoensurethatapplicationsarealwaysavailableandresponsive.
• Security: Containers provide a level of isolation between applications that can help improve
security. Containerscanbeusedtorunapplicationsinasandboxedenvironmentthatisisolated
fromotherapplications. Thiscanhelp preventapplicationsfrominterfering witheach otherand
canhelppreventsecurityvulnerabilitiesfrombeingexploited.
ThedifferenceswiththeVMsare:
• ContainersaremorelightweightthanVMs
• ContainersarefastertostartandstopthanVMs
• ContainersuselessmemorythanVMs
• ContainersaremoreportablethanVMs
• ContainersareeasiertomanagethanVMs
YoucanfindapracticalexampleofthesedifferencesinmyGitHubrepository: ClusterAnalysis.
Draft
6.2 Docker
Docker is a containerization platform that packages your application and all its dependencies
togetherintheformofadockercontainertoensurethatyourapplicationworksseamlesslyinany
environment.
BOOKDefinition: ContainerEngine
Acontainerengineisapieceofsoftwarethatacceptsuserrequests,includingcommandline
options,pullsimages,andfromtheenduser’sperspectiverunsthecontainer. Itisreponsible
for:
• Buildingimages
• Runningcontainers
• Stoppingcontainers
• Removingcontainers
• Managingnetworks
• Managingvolumes
Docker Container is a standardized unit which can be created on the fly to deploy a particular
applicationorenvironment. ItcouldbeanUbuntucontainer,CentOscontainer,etc. tofull-fillthe
requirementfromanoperatingsystempointofview.
Containerscreateanisolatedenvironmentatapplicationlevel: eachcontainerisaprocessrunning
includingallitschildren;
Chapter6. Containers 32

Containersunderlying(main)technologies:
• LinuxNAMESPACES:Isolatessystemresources
• LinuxCGroups: Limitsresources
Userspacereferstoallthecodeinanoperatingsystemthatlivesoutsideofthekernel.
Figure6.2: processVirtualization
6.2.1 Linux Namespaces
Linuxnamespacesprovideamechanismforisolatingsystemresources,enablingprocesseswithina
namespacetohavetheirownviewofthesystem,suchasprocessIDs,NamespacesinLinuxprovide
awaytoisolateandvirtualizesystemresources,thusenhancingsecuritybypreventingprocesses
in one namespace from directly interacting with processes in another namespace. Namespaces
increasesecuritybyprovidingalevelofisolationthatpreventsunintendedinteractionsbetween
processes. Thisisolationisparticularlyvaluableincontainerizationandvirtualizationscenarios,
Draft
wheremultipleapplicationsorservicessharethesamehostsystembutmustbekeptseparatefor
securityreasons.
• A user namespace has its own set of user IDs and group IDs for assignment to processes. In
particular,thismeansthataprocesscanhaveroot privilegewithin itsusernamespace without
havingitinotherusernamespaces.
• AprocessID(PID)namespaceassignsasetofPIDstoprocessesthatareindependentfrom
the set of PIDs in other namespaces. The first process created in a new namespace has PID 1
andchildprocessesareassignedsubsequentPIDs. IfachildprocessiscreatedwithitsownPID
namespace,ithasPID1inthatnamespaceaswellasitsPIDintheparentprocess’namespace.
• A network namespace has an independent network stack: its own private routing table, set
of IP addresses, socket listing, connection tracking table, firewall, and other network-related
resources.
• A mount namespace has an independent list of mount points seen by the processes in the
namespace. This means that you can mount and unmount filesystems in a mount namespace
withoutaffectingthehostfilesystem.
Within the parent namespace, there are four processes, named PID1 through PID4. These are
normalprocesseswhichcanallseeeachotherandshareresources.
Chapter6. Containers 33

|     |     | Figure6.3: | PIDs |     |
| --- | --- | ---------- | ---- | --- |
Totestnamespaces,youcanusethefollowingcommands:
1
# Show all processes
2
sudo ps -ef
3
4
# Show all namespaces
5
sudo lsns
6
7
|     | # Create a new PID   | namespace    |      |     |
| --- | -------------------- | ------------ | ---- | --- |
| 8   | unshare --fork --pid | --mount-proc | bash |     |
9
10
|     | # Create a new network | namespace |     |     |
| --- | ---------------------- | --------- | --- | --- |
11
|     | ip netns add test |     |     |     |
| --- | ----------------- | --- | --- | --- |
12
|     | ip netns exec test | bash |     |     |
| --- | ------------------ | ---- | --- | --- |
Draft
Namespacesareoneofthetechnologiesthatcontainersarebuilton,usedtoenforcesegregationof
resources. We’veshownhowtocreatenamespacesmanually,butcontainerruntimeslikeDocker,
rkt,andpodmanmakethingseasierbycreatingnamespacesonyourbehalf.
| 6.2.2 | Linux CGroups |     |     |     |
| ----- | ------------- | --- | --- | --- |
Acontrolgroup(cgroup)isaLinuxkernelfeaturethatlimits,accountsfor,andisolatestheresource
usage(CPU,memory,diskI/O,network,andsoon)ofacollectionofprocesses.
Mainfeaturesare:
• Resourcelimiting: Setlimitsontheamountofresourcesagroupofprocessescanuse.
• Prioritization: Givecertaingroupsofprocesseshigherorlowerpriority.
• Accounting: Keeptrackoftheresourcesusedbyagroupofprocesses.
| • Control: | Freeze,resume,orkillagroupofprocesses. |     |     |     |
| ---------- | -------------------------------------- | --- | --- | --- |
Kerneluses cgroupsto control howmuch of agiven key resource(CPU, memory, network, and
diskI/O)canbeaccessedorusedbyaprocessorsetofprocesses.
| Chapter6. | Containers |     |     | 34  |
| --------- | ---------- | --- | --- | --- |

|     |     |     | Figure6.4: | CGroups |     |     |
| --- | --- | --- | ---------- | ------- | --- | --- |
Youcanaccesscgroupswithvarioustools:
1
|     | # Show all | cgroups |     |     |     |     |
| --- | ---------- | ------- | --- | --- | --- | --- |
2
sudo lscgroup
3
Draft
| 4   | # Show cgroup | hierarchy |     |     |     |     |
| --- | ------------- | --------- | --- | --- | --- | --- |
5
systemd-cgtop
6
7
|     | # Create | a new cgroup |     |     |     |     |
| --- | -------- | ------------ | --- | --- | --- | --- |
8
|     | sudo cgcreate | -g memory:mygroup |     |     |     |     |
| --- | ------------- | ----------------- | --- | --- | --- | --- |
9
10
|     | # Set memory | limit |     |     |     |     |
| --- | ------------ | ----- | --- | --- | --- | --- |
11
|     | sudo cgset | -r memory.limit_in_bytes=1G |     |     | mygroup |     |
| --- | ---------- | --------------------------- | --- | --- | ------- | --- |
12
13
|     | # Add a         | process to | a cgroup       |      |     |     |
| --- | --------------- | ---------- | -------------- | ---- | --- | --- |
| 14  | sudo cgclassify | -g         | memory:mygroup | $PID |     |     |
15
16
# Control groups can be implemented directly by container engines
17
|       | docker run | -d --name | mycontainer | --memory | 1g myimage |     |
| ----- | ---------- | --------- | ----------- | -------- | ---------- | --- |
| 6.2.3 | Docker     | File      |             |          |            |     |
To build a Docker image, you need to create a Dockerfile. A Dockerfile is a text document that
contains all the commands a user could call on the command line to assemble an image. Using
dockerbuildusers cancreate anautomatedbuildthat executesseveralcommand-line instructions
insuccession.
| Chapter6. | Containers |     |     |     |     | 35  |
| --------- | ---------- | --- | --- | --- | --- | --- |

|     |     | Figure6.5: | Dockerfile |     |
| --- | --- | ---------- | ---------- | --- |
TheDockerfilehasthefollowingstructure:
• FROM: This instruction in the Dockerfile tells the daemon, which base image to use while
creating our new Docker image. In the example here, we are using a very minimal OS image
calledalpine(just5MBofsize). YoucanalsoreplaceitwithUbuntu,Fedora,Debianorany
otherOSimage.
• RUN: This command instructs the Docker daemon to run the given commands as it is while
creating the image. A Dockerfile can have multiple RUN commands, each of these RUN
commandscreateanewlayerintheimage.
• ENTRYPOINT:The ENTRYPOINT instructionis used whenyou would like yourcontainer to
run the same executable every time. Usually, ENTRYPOINT is used in scenarios where you
wantthecontainertobehaveexclusivelyasifitwerethe executableit’swrapping.
• CMD:TheCMDsetsdefaultcommandsand/orparameterswhenadockercontainerruns. CMD
canbeoverwrittenfromthecommandlineviathedockerruncommand.
|     | Question-CircleExample: |     | Draft |     |
| --- | ----------------------- | --- | ----- | --- |
Dockerfile
1
|     | FROM ubuntu:18.04 |        |     |     |
| --- | ----------------- | ------ | --- | --- |
| 2   | RUN apt           | update |     |     |
3
|     | RUN apt | upgrade |     |     |
| --- | ------- | ------- | --- | --- |
4
|     | ENTRYPOINT | ["echo", | "Hello, World!"] |     |
| --- | ---------- | -------- | ---------------- | --- |
You can create multiple containers from the same image, and each container will have its own
isolated environment. You can also create multiple images from the same Dockerfile, and each
imagewillhaveitsownsetoflayers.
Dockerimagesarebasedonlayers. Eachlayerisasetofread-onlyfilesthatiscreatedwhenthe
imageisbuilt. Whenyourunacontainerfromanimage,Dockercreatesaread-writelayerontop
oftheimagelayers. Thisread-writelayeriswherethecontainerwritesdata.
|           |            | Figure6.6: | DockerLayers |     |
| --------- | ---------- | ---------- | ------------ | --- |
| Chapter6. | Containers |            |              | 36  |

7
Data Cloud and Cloud Security
7.1 Data Cloud
CloudStorageisaservicemodel inwhichdata ismaintained,managedandbackedup remotely
andmadeavailabletousersoveranetwork(typicallytheInternet). Usersgenerallypayfortheir
clouddatastorageonaper-consumption,monthlyrate. Althoughtheper-gigabytecosthasbeen
radicallydriven down,cloud storageproviders haveadded operatingexpenses thatcan makethe
technology more expensive than users bargained for. Cloud security continues to be a concern
amongusers. Providershavetriedtoaddressthosefearsbybuildingsecuritycapabilities,suchas
encryptionandauthentication,intotheirservices.
Traditionalstoragesare:
BlockStorage:
• Volumes
• Blocks(read/write)
• FibreChanneloriSCSIprotocols
• Local
• Lowlatency,highIOPs,lowsize(<1PB) Draft
• Complextoexpandandexpensive
Figure7.1: BlockStorage
FileStorage:
• Files
• NFS,SMB,FTP,etc.
• Local
• Lowlatency,highIOPs,lowsize(<1PB)
• Complextoexpandandexpensive
Figure7.2: FileStorage
7.1.1 Distributed File System
BOOKDefinition: DistributedFileSystem
ADistributedFileSystemisasharedfilesystemthatallowsmanyclientstoaccessfiles
anddirectories storedon acentralserver. Itis aclient/server-basedapplication that allows
clientstoaccessandprocessdatastoredontheserverasifitwereontheirowncomputer.
• Client: Theclientistheend-userdevicethataccessesthefilesstoredontheserver.
• Server: The server is the computer that stores the files and directories that the clients
access.
Properties:
Chapter7. DataCloudandCloudSecurity 37

• Transparency: A client cannot tell where a file is located. A file can transparently move to
anotherserver. Multiplecopiesofafilemayexist. Multipleclientsaccessthesame file.
• Flexibility: Serversmaybeaddedorreplacedandthereissupportformultiplefilesystemtypes.
• Dependability: Conflictswith replicationandconcurrency control. Users mayhave different
accessrightsonclientssharingfilesandnetworktransmission. Servercrashesmaycausedata
loss.
• Performance: Requests may be distributed across servers and multiple servers allow higher
storacecapacity.
• Caching: Reducenetworktrafficbyretainingrecentlyaccesseddiskblocksinacache,sothat
repeatedaccessestothesameinformationcanbehandledlocally. Ifrequireddataisnotalready
cached,acopyofdataisbroughtfromtheservertotheuser.
Configurationand implementation may vary, serversmay runon dedicated machines orservers
andclientscanbeonthesamemachines.
Draft
Figure7.3: DistributedFileSystem
Exclamation-TriangleWarning: Limitationsoftraditionalstorage
Traditionalstoragesystemsarelimitedintermsofscalability,performance,andcost. Theyare
notdesignedtohandlethemassiveamountsofdatathataregeneratedbymodernapplications.
They are also not designed to handle the high levels of concurrency that are required by
modernapplications. Inaddition, traditionalstoragesystems areexpensivetopurchase and
maintain. They require a significant amount of hardware and software to be installed and
configured,andtheyrequireadedicatedteamofITprofessionalstomanagethem.
Question-CircleExample: GoogleFileSystem
TheGoogleFileSystem(GFS)isadistributedfilesystemthatwasdevelopedbyGoogleto
handlethemassiveamountsofdatathataregeneratedbyitssearchengine. GFSisdesigned
tobehighly scalable, highlyavailable, andhighly reliable. Itis designedto handlethe high
levelsofconcurrencythatarerequiredbymodernapplications. GFSisalsodesignedtobe
cost-effective,withalowtotalcostofownership. GFSisbasedonamaster/slavearchitecture,
withasinglemasterserverthatmanagesthemetadataforthefilesystemandmultipleslave
servers that store the actual data. GFS uses a distributed lock service to manage access to
Chapter7. DataCloudandCloudSecurity 38

the file system, and it uses a distributed file system protocol to communicate between the
masterandslaveservers. GFSisdesignedtobefault-tolerant,withmultiplecopiesofeach
filestoredondifferentserverstoensurethatdataisnotlostintheeventofaserverfailure.
GFSisalsodesignedtobehighlyavailable,withmultiplemasterserversthatcantakeover
intheeventofamasterserverfailure. GFSisdesignedtobehighlyreliable,withbuilt-in
mechanismsfordetectingandrecoveringfromdatacorruptionandothererrors.
• MasterServer: Singlemasterserverthatmanagesthe metadataforthefilesystem.
• ChunkServers: Multipleslaveserversthatstoretheactualdata.
Figure7.4: GoogleFileSystemArchitecture
Draft
Figure7.5: ReadFiles
Anobjectisalogicalunitofstoragethatisstoredonaserver. Anobjecthasauniqueidentifier,a
setofattributes,andasetofmethodsthatcanbeusedtoaccessandmanipulatetheobject. Objects
aretypicallystoredinadistributedfilesystem,wheretheyarereplicatedacrossmultipleserversto
ensurehighavailabilityanddurability. ObjectsaretypicallyaccessedusingaRESTfulAPI,which
allowsclientstoperformCRUDoperationsontheobjectsusingstandardHTTPmethods.
Metadata,instead,isdatathatdescribestheattributesofanobject. Metadataistypicallystoredin
aseparatedatabaseorfilesystemfromtheobjectitself,andisusedtostoreinformationsuchasthe
object’sname,size,type,andlocation. MetadataistypicallyaccessedusingaseparateAPIfrom
theobjectitself,whichallowsclientstoqueryandupdatethemetadatawithouthavingtoaccess
theobjectitself.
EYEObservation: FITSfiles
TheFlexibleImageTransportSystem(FITS)isanopenstandarddefiningadigitalfileformat
useful for storage, transmission and processing of data: formatted as multi-dimensional
arrays(images)ortables. FITSisthemostcommonlyuseddigitalfileformatinastronomy.
FITS is much more than just another image format (such as JPG or GIF) and is primarily
designedtostorescientificdatasetsconsistingofmulti-dimensionalarrays(2Dtables,3D
Chapter7. DataCloudandCloudSecurity 39

datacubes,etc.) and1Dtablesconsistingofcolumnsandrows. FITSisalsooftenusedto
store non-image data, such as spectra, photon lists, data cubes, or structured data such as
objectcatalogs.
Requestfor storageare madewith HTTPusingRESTful APIs. Therequestismade tothe server,
whichisresponsibleforstoringthedata. Theserverthenstoresthedatainadistributedfilesystem,
which replicates the data across multiple servers to ensure high availability and durability. The
serverthenreturnsaresponsetotheclient,whichindicateswhethertherequestwassuccessfulor
not. Primarycomponentsofarequestare:
1. HTTPVerb: GET,POST,PUT,DELETE
2. AuthenticationInformation
3. SURL:StorageURL
4. Metadata(optional)
Draft
Figure7.6: RequestforStorage
Figure7.7: manageObjectwithHTTP
Chapter7. DataCloudandCloudSecurity 40

7.1.2 Cloud Storage capabilities
BOOKDefinition: CloudStorage
Cloud Storage is a service model in which data is maintained, managed and backed up
remotelyandmadeavailabletousersoveranetwork(typicallytheInternet). Usersgenerally
pay for their cloud data storage on a per-consumption, monthly rate. Although the per-
gigabytecosthasbeenradicallydrivendown,cloudstorageprovidershaveaddedoperating
expenses that can make the technology more expensive than users bargained for. Cloud
securitycontinuestobeaconcernamongusers. Providershavetriedtoaddressthosefears
bybuildingsecuritycapabilities,suchasencryptionandauthentication,intotheirservices.
Itscapabilitiesare:
• Cloud File Storage: Store and manage files in the cloud. Files are stored in a distributed
file system, which replicates the data across multiple servers to ensure high availability and
durability. Files are typically accessed using a RESTful API, which allows clients to perform
CRUDoperationsonthefilesusingstandardHTTPmethods.
• User management: Manage users and their access to files. Users are typically authenticated
usingausername andpassword, andtheir accessto filesis controlledusing accesscontrol lists
(ACLs). Userscanbeassigneddifferentrolesandpermissions,whichdeterminewhatactions
theycanperformonthefiles.
• EasytouseGUI:Agraphicaluserinterface(GUI)thatallowsuserstoeasilyupload,download,
and manage files in the cloud. The GUI typically provides a file browser that allows users to
navigatethefilesystem,andasetofbuttonsthatallowuserstoperformcommonfileoperations.
• Security: Security features such as encrypDtiroanf,t authentication, and access control. Files are
typically encrypted at rest and in transit, to protect them from unauthorized access. Users are
authenticatedusingausernameandpassword,andtheiraccesstofilesiscontrolledusingaccess
controllists(ACLs).
• Scalability: Scalabilityfeaturessuchasautomaticscalingandloadbalancing. Thecloudstorage
system is designed to automatically scale to handle large amounts of data and high levels of
concurrency. Load balancing ensures thatrequestsare distributed evenly across theservers,to
preventanyoneserverfrombecomingoverloaded.
• Multipleaccessprotocols: Supportformultipleaccessprotocols,suchasRESTfulAPIs,NFS,
and SMB. Users can access files using a variety of protocols, depending on their needs and
preferences. RESTfulAPIsaretypicallyusedforprogrammaticaccess,whileNFSandSMBare
typicallyusedforfilesharingandcollaboration.
Itsarchitectureisbasedonadistributedfilesystem,whichreplicatesthedataacrossmultipleservers
toensurehigh availabilityand durability. Thedistributed filesystemis typicallyaccessed usinga
RESTfulAPI,which allowsclients toperform CRUDoperations onthe filesusingstandardHTTP
methods. Thedistributedfilesystemistypicallyimplementedusingamaster/slavearchitecture,
withasinglemasterserverthatmanagesthemetadataforthefilesystemandmultipleslaveservers
thatstoretheactualdata.
Chapter7. DataCloudandCloudSecurity 41

Figure7.8: CloudStorageArchitecture
Draft
Figure7.9: ProductionDeployment
Tomonitorthesystemseveraltoolscanbeused:
• Dockerstats: Command linetool that provides real-time resource usage statisticsfor Docker
containers.
• Portainer: ManagementtoolforDockerenvironments,easytosetup.
• Grafana: Analyticsandvisualizationplatformthatallowsuserstoquery,visualizeandmonitor
datafrommultiplesourcesinreal-time. Requiresconfiguration.
Chapter7. DataCloudandCloudSecurity 42

7.2 Cloud Security
Figure7.10: CloudSecurity
Differenttypesofcloudcomputingservicemodelsprovidedifferentlevelsofsecurity. Thereare
threelevelsofcloudsecurity:
• Network Level: Security at the network level is the most basic level of security. It involves
securingthenetworkinfrastructurethatconnDercatfstthecloudserverstotheInternet. Thisincludes
firewalls,intrusion detectionsystems, andother networksecurity devices. Networksecurity is
importantbecauseitprotectsthecloudserversfromattacksthatoriginatefromtheInternet.
• Host Level: Security at the host level is the next level of security. It involves securing the
individualserversthatmakeupthecloudinfrastructure. Thisincludessecuringtheoperating
system, the applications that run on the server, and the data that is stored on the server. Host
securityisimportantbecauseitprotectsthecloudserversfromattacksthatoriginatefromwithin
thecloudinfrastructure.
• ApplicationLevel: Security at the application level isthehighest level of security. It involves
securing the applications that run on the cloud servers. This includes securing the code that
makesuptheapplication,thedatathattheapplicationprocesses,andtheusersthat accessthe
application. Applicationsecurityisimportantbecauseitprotectsthecloudserversfromattacks
thatexploitvulnerabilitiesintheapplicationcode.
So,beforeapproachingthecloud,itisimportanttounderstandthesecurityrisksandchallengesthat
comewithit. Themostcommonsecurityrisksandchallengesassociatedwithcloudcomputingare:
• DataBreaches: Databreachesareoneofthemostcommonsecurityrisksassociatedwithcloud
computing. A data breach occurs when an unauthorized party gains access to sensitive data
storedinthecloud. Thiscan happenthroughavarietyofmeans,such ashacking,phishing, or
socialengineering.
• DataLoss: Datalossisanothercommonsecurityriskassociatedwithcloudcomputing. Data
loss occurs when data stored in the cloud is accidentally deleted, corrupted, or otherwise lost.
Thiscanhappenduetoavarietyofreasons,suchashardwarefailure,softwarebugs,orhuman
error.
• Account Hijacking: Account hijacking is a security risk that occurs when an unauthorized
Chapter7. DataCloudandCloudSecurity 43

partygainsaccesstoauser’scloudaccount. Thiscanhappenthroughavarietyofmeans,such
asphishing, socialengineering, orweak passwords. Onceanattackergains accesstoa user’s
account,theycanstealsensitivedata,deletefiles,orperformothermaliciousactivities.
• InsecureAPIs: InsecureAPIsareasecurityriskthatoccurswhentheAPIsusedtoaccesscloud
services are not properly secured. This can happen due to a variety of reasons, such as weak
authenticationmechanisms,lackofencryption,orothervulnerabilitiesintheAPIimplementation.
Insecure APIs can be exploited by attackers to gain unauthorized access to sensitive data or
performothermaliciousactivities.
• Insider Threats: Insider threats are a security risk that occurs when an authorized user of a
cloudserviceintentionallyor unintentionallycausesharm totheservice. Thiscanhappendue
to a variety of reasons, such as disgruntled employees, careless users, or users who are tricked
by attackers. Insider threats can be difficult to detect and prevent, as the attacker already has
legitimateaccesstotheservice.
Host Level
• Hypervisorsecurity: The hypervisor is a critical component ofthe cloud infrastructure, as it
isresponsibleformanagingthevirtualmachinesthatrunonthephysicalservers. Hypervisor
securityisimportantbecauseacompromiseofthehypervisorcanleadtoacompromiseofallthe
virtualmachinesthatrunonit. Hypervisorsecuritycanbeimprovedbyusingsecurehypervisors,
keepingthehypervisoruptodate,andusingstrongaccesscontrols.
• VirtualMachinessecurity: Virtualmachinesarethebuildingblocksofthecloudinfrastructure,
astheyruntheapplicationsthatmakeupthecloudservices. Virtualmachinesecurityisimportant
becauseacompromiseofavirtualmachinecanleadtoacompromiseofthedataandapplications
thatrunonit. VirtualmachinesecuritycanbDeriamftprovedbyusingsecurevirtualmachineimages,
keepingthevirtualmachinesuptodate,andusingstrong accesscontrols. Alsosshkeyscanbe
usedtoaccessthevirtualmachines.
TheVM/Containerssecurityhastheadvantagesofasimplerimplementationofresourcemanage-
mentpolicies,alongwithimprovedintrusionpreventionanddetectionandmoreefficientsoftware
testing.
TheOSimplementminimalsecurityon: Accesscontrol,authenticationusageandcryptographic
usage policies. Applications with special privilegies that perform security-related functions are
calledtrusted applications. They shouldonlybe allowedin thelowest levelof privilegesrequired
to perform their functions. An OS poorly isolates one application from another, and once an
applicationiscompromised,theentirephysicalplatformandallapplicationsrunningonitcanbe
affected.
ForDataSecurity,Identifythesecurityboundaryseparatingtheclient’sandvendor’sresponsi-
bilities Determine the sensitivity of the data to risk Data should be transferred and stored in an
encryptedformat. Separateclientsfromdirectaccesstosharedcloudstorage.
Thefollowingarethemechanismforprotectingdata:
• Accesscontrol,whichistheprocessofdeterminingwhocanaccesswhatdataandunderwhat
conditions. Access control is typically implemented using a combination of authentication,
authorization,andauditingmechanisms.
• Auditing,whichistheprocessofmonitoringandrecordingaccesstodata. Auditingistypically
implementedusingacombinationoflogging,monitoring,andreportingmechanisms.
• Authentication, which is the process of verifying the identity of a user. Authentication is
typicallyimplementedusingacombinationofpasswords,biometrics,andotherauthentication
mechanisms.
Chapter7. DataCloudandCloudSecurity 44

• Authorization, which is the process of determining what data a user can access. Authorization
istypicallyimplementedusingacombinationofaccesscontrollists,role-basedaccesscontrol,
andotherauthorizationmechanisms.
Youshouldisolatedatafromdirectclientaccess,creatingalayeredaccesstoit. Usedatasegregation
basedontenants.
Draft
Figure7.11: DataSecurity
Mostclooudserviceprovidersstoredatainanencryptedformonserversideorclientside.
Problems:
• aproblemwithencrypteddatamayresultwithdatathatmaynotberecoverable.
• itdoesnothingtopreventdataloss: keepyourkeys!
Chapter7. DataCloudandCloudSecurity 45