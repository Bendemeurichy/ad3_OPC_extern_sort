
---
title: "sorteren met orde-bewarende prefix-codering"
course: AD3
year: 2023-2024
---

# Opgave project AD3: sorteren met orde-bewarende prefix-codering

Eén van de onderzoeksprojecten aan de vakgroep WE02 is [Unipept](https://unipept.ugent.be/): een webapplicatie waar onderzoekers hun biologische data kunnen analyseren.
Een belangrijk onderdeel van Unipept is het zoeken van peptiden in een databank.
Die peptiden zijn de "basisblokken" van eiwitten en bestaan uit een sequentie van aminozuren die kunnen voorgesteld worden als een string van letters.
De dataset die Unipept gebruikt bevat echter een hele hoop peptidesequenties, die in totaal ongeveer 900 GB aan data in beslag neemt.

Om je een idee te geven van hoe deze data er uit ziet:
het "spike-eiwit", dat een belangrijke rol speelt bij het SARS-CoV-2-virus, bestaat uit onderstaande sequentie (opgesplitst in kolommen en lijnen voor de leesbaarheid):

```
MFIFLLFLTL TSGSDLDRCT TFDDVQAPNY TQHTSSMRGV YYPDEIFRSD TLYLTQDLFL
PFYSNVTGFH TINHTFGNPV IPFKDGIYFA ATEKSNVVRG WVFGSTMNNK SQSVIIINNS
TNVVIRACNF ELCDNPFFAV SKPMGTQTHT MIFDNAFNCT FEYISDAFSL DVSEKSGNFK
HLREFVFKNK DGFLYVYKGY QPIDVVRDLP SGFNTLKPIF KLPLGINITN FRAILTAFSP
AQDIWGTSAA AYFVGYLKPT TFMLKYDENG TITDAVDCSQ NPLAELKCSV KSFEIDKGIY
QTSNFRVVPS GDVVRFPNIT NLCPFGEVFN ATKFPSVYAW ERKKISNCVA DYSVLYNSTF
FSTFKCYGVS ATKLNDLCFS NVYADSFVVK GDDVRQIAPG QTGVIADYNY KLPDDFMGCV
LAWNTRNIDA TSTGNYNYKY RYLRHGKLRP FERDISNVPF SPDGKPCTPP ALNCYWPLND
YGFYTTTGIG YQPYRVVVLS FELLNAPATV CGPKLSTDLI KNQCVNFNFN GLTGTGVLTP
SSKRFQPFQQ FGRDVSDFTD SVRDPKTSEI LDISPCSFGG VSVITPGTNA SSEVAVLYQD
VNCTDVSTAI HADQLTPAWR IYSTGNNVFQ TQAGCLIGAE HVDTSYECDI PIGAGICASY
HTVSLLRSTS QKSIVAYTMS LGADSSIAYS NNTIAIPTNF SISITTEVMP VSMAKTSVDC
NMYICGDSTE CANLLLQYGS FCTQLNRALS GIAAEQDRNT REVFAQVKQM YKTPTLKYFG
GFNFSQILPD PLKPTKRSFI EDLLFNKVTL ADAGFMKQYG ECLGDINARD LICAQKFNGL
TVLPPLLTDD MIAAYTAALV SGTATAGWTF GAGAALQIPF AMQMAYRFNG IGVTQNVLYE
NQKQIANQFN KAISQIQESL TTTSTALGKL QDVVNQNAQA LNTLVKQLSS NFGAISSVLN
DILSRLDKVE AEVQIDRLIT GRLQSLQTYV TQQLIRAAEI RASANLAATK MSECVLGQSK
RVDFCGKGYH LMSFPQAAPH GVVFLHVTYV PSQERNFTTA PAICHEGKAY FPREGVFVFN
GTSWFITQRN FFSPQIITTD NTFVSGNCDV VIGIINNTVY DPLQPELDSF KEELDKYFKN
HTSPDVDLGD ISGINASVVN IQKEIDRLNE VAKNLNESLI DLQELGKYEQ YIKWPWYVWL
GFIAGLIAIV MVTILLCCMT SCCSCLKGAC SCGSCCKFDE DDSEPVLKGV KLHYT
```

Het alfabet dat hier gebruikt wordt is relatief bepekt: er zijn in totaal ongeveer 24 verschillende aminozuren mogelijk, dus gebruiken we niet het volledige ASCII-alphabet.
Daarnaast komen sommige peptiden veel vaker voor dan andere.
Dit betekent dat deze data eigenlijk goed gecomprimeerd kan worden.

Om die data efficiënt te kunnen doorzoeken, worden deze sequenties op voorhand gesorteerd.
Deze strings passen echter niet allemaal tegelijkertijd in het geheugen, dus moeten we **extern sorteren** toepassen, waarbij we onze data opsplitsen in kleinere stukken die wel nog in het geheugen passen.
De snelheid van het sorteren hangt af van de grootte van de stukken die we in het geheugen kunnen houden.

Verder willen we nu deze data **comprimeren** zodat we minder geheugen nodig hebben om de data op te slaan, maar willen we ze ook nog steeds kunnen **sorteren** zonder ze eerst te decomprimeren.
In dit project zullen we onderzoeken hoe goed een orde-bewarende versie van prefix-codering werkt voor deze data.
We leggen dus de extra voorwaarde op dat de codewoorden van onze prefix-code lexicografisch dezelfde volgorde hebben als het letterteken dat ze voorstellen.

## Functionele vereisten programma

Schrijf een command-line programma dat een bestand kan inlezen, comprimeren, decomprimeren en sorteren.
Hierbij moet telkens gebruik gemaakt worden van een orde-bewarende prefix-codering.

Deze orde-bewarende prefix-codering (OPC) is een prefix-code die voldoet aan de definitie uit de cursus (definitie 23) met de bijkomende voorwaarden dat de codewoorden lexicografisch dezelfde volgorde hebben als het teken die ze coderen én dat de codering van de invoer-tekst resulteert in een zo klein mogelijk aantal bits onder deze voorwaarde.

Omdat we de lijnen willen sorteren en we dus de grenzen van de lijn gemakkelijk willen kunnen terugvinden, kan je er voor kiezen om een newline (`\n`) niet op te nemen in de prefix-codering. In dat geval moet je op een andere manier kunnen bepalen waar een lijn start of stopt.

## Specificaties

### Programmeertaal

In de opleidingscommissie informatica (OCI) werd beslist dat, om meer ervaring met het programmeren in C te verwerven, het project van Algoritmen en Datastructuren 3 in C geïmplementeerd moet worden.
Het is met andere woorden de bedoeling je implementatie in C uit te voeren. Je implementatie moet voldoen aan de ANSI-standaard.
Je mag hiervoor dus gebruikmaken van de laatste features in C17, voor zover die ondersteund worden door de `C` compiler die gebruikt wordt voor dit project op SubGIT (GCC 12.2).

Voor het project kun je de standaard libraries gebruiken, externe libraries
zijn echter niet toegelaten. Het spreekt voor zich dat je normale, procedurele
C-code schrijft en geen platformspecifieke API's (zoals bv. de Win32 API) of
features uit C++ gebruikt. Op Windows bestaat van enkele functies zoals `qsort`
een "safe" versie (in dit geval `qsort_s`), maar om je programma te kunnen
compileren op een Unix-systeem kun je die versie dus niet gebruiken. Let er ook op dat je programma geen geheugenlekken bevat.

**Wat je ontwikkelingsplatform ook mag zijn, controleer geregeld de output van
SubGIT om te verifiëren dat je programma ook bij ons compileert en slaagt voor de minimale testen!**

### Commando's

Je programma moet de volgende commando's ondersteunen. Dit commando zal als tweede argument meegegeven worden aan jouw programma.

- `tree`: tel hoeveel keer elk karakter voorkomt in een bestand, bereken de optimale orde-bewarende prefix-codering (OPC) en schrijf deze uit naar een bestand (volgens het formaat dat we hieronder beschrijven).
- `compress`: comprimeer een bestand met een orde-bewarende prefix-codering. Je mag daarbij zelf je eigen formaat bepalen, maar dat moet goed gedocumenteerd zijn in je verslag.
- `extract`: decomprimeer een bestand dat gecomprimeerd werd met jouw programma.
- `sort`: sorteer een bestand dat gecomprimeerd werd met jouw programma, het resultaat moet nog steeds decomprimeerbaar zijn met jouw programma. Het volledige bestand past misschien niet in het geheugen, je zal dus **extern sorteren** moeten implementeren.
Optioneel mag je ook 'gewoon' sorteren implementeren. Dan mag je veronderstellen dat het volledige bestand in het geheugen past en leggen we geen beperkingen op je geheugengebruik. De maximale score voor het project is dan 3 op 4 punten.

Elk commando vereist drie opties:

- `-i`: het pad naar het inputbestand.
- `-o`: het pad naar het outputbestand.
- `-m`: de grootte van de buffer die je mag gebruiken om de ingelezen strings in op te slaan, in bytes.

Het programma `compress` heeft nog één extra optie:
`-t`: een pad naar een bestand met de prefix-codering. Dit bestand is gegenereerd door het `tree` commando waarvan het formaat hieronder staat beschreven.

Je mag zeker extra opties toevoegen, maar deze moeten optioneel zijn.
Je programma zal getest worden met de bovenstaande opties.

#### Output van het `tree` commando

Voor elk karakter dat voorkomt in het bestand, schrijf je een lijn uit naar het outputbestand in het volgende formaat:

```
<karakter-waarde (decimaal)> <aantal voorkomens (decimaal)> <orde-bewarende prefix-code (binair)>
```

Misschien wil je er voor kiezen om newlines (`\n`) niet te coderen omdat je een andere manier hebt om de grenzen tussen lijnen te bepalen.
In dat geval mag je het veld waar de binaire voorstelling van de orde-bewarende prefix-code die de newline codeert, leeg laten.
Alle andere aanwezige karakters moeten een binaire prefix-code hebben.

Bijvoorbeeld, gegeven het volgende inputbestand:

```
VCELEGWTLEK
LELLPLEGTLYEK
CLACARPGR
```

Is dit een mogelijke output:

```
10 3 
65 2 000
67 3 001
69 6 010
71 3 0110
75 2 0111
76 8 10
80 2 1100
82 2 1101
84 2 1110
86 1 11110
87 1 111110
89 1 111111
```

Elke lijn eindigt met een newline `\n` met 10 als decimale waarde. In ons programma kiezen we ervoor om newlines niet te coderen, dus eindigt de lijn zonder binaire code.

Het karakter `A` heeft decimale waarde `65` en komt 2 keer voor, `A` wordt lexicografisch eerst gesorteerd en heeft dus een prefix-code met alleen maar `0`-bits.

`76` is de decimale waarde van het karakter `L` dat het vaakst (8 keer) voorkomt en heeft in dit voorbeeld de kortste prefix-code.

`89` is de decimale waarde van het karakter `Y` dat maar één keer voorkomt en lexicografisch laatst gesorteerd wordt, hierdoor heeft het een lange prefix-code met allemaal `1`-bits.

**Opmerking:** het is mogelijk dat jouw prefix-code afwijkt van wat er in bovenstaand voorbeeld staat.

### Datasets

In het mapje `data/` vind je scriptjes om datasets te downloaden of te genereren. Dit zijn (delen van) de échte data die we gebruiken om de Unipept-databank op te bouwen.

Tijdens het verbeteren zullen we gelijkaardige bestanden gebruiken om jullie tijd- en geheugengebruik mee te meten. Het loont dus zeker de moeite om voor deze bestanden te optimaliseren.
Je moet echter ook "gewone" tekstbestanden kunnen comprimeren en sorteren.

#### Bestandsformaat

Je programma moet bestanden kunnen comprimeren en sorteren die voldoen aan de volgende specificaties:

- Elk bestand bestaat uit 8-bit karakters uit de ASCII-standaard, maar zal nooit een null-byte (`\0`) bevatten (dus karakters kunnen een byte-waarde hebben van 1 tot en met 127).
- Elk bestand zal bestaan uit een aantal lijnen gescheiden door een newline (`\n`) karakter.
- Een lijn zal nooit langer zijn dan 45354 karakters (de langste sequentie in de [TrEMBL](https://www.ebi.ac.uk/uniprot/TrEMBLstats)-databank)
- Het gemiddelde van de lengtes van de lijnen in een bestand zal niet kleiner zijn dan 8 karakters.
- De lijnen worden gesorteerd volgens de lexicografische volgorde van de karakters (geïnterpreteerd als `unsigned char`), je moet dus geen rekening houden met [taal- en cultuurafhankelijke sorteervolgorde](http://unicode.org/reports/tr10/#Introduction).
- De frequenties van karakters in een bestand zullen er nooit voor zorgen dat een prefix-code voor één karakter meer dan 64 bits zal nodig hebben voor de optimale orde-bewarende prefix-codering.
- Een gecodeerde lijn zal ook nooit langer zijn dan 45354 bytes (de langst mogelijke niet-gecodeerde lijn). Lijnen zullen ook meestal veel korter te coderen zijn dan de originele lijn.

Je programma mag natuurlijk ook bestanden kunnen afhandelen die niet aan deze specificaties voldoen, maar dat hoeft niet.

De datasets waarvoor je zal optimaliseren hebben de bijkomende eigenschappen dat ze maar een beperkt aantal verschillende karakters bevatten en dat sommige karakters veel vaker voorkomen dan andere.

### Limieten

We voorzien enkele testen die de basisfunctionaliteit van je programma testen.
Je moet tenminste voor al deze testen slagen om punten te krijgen voor het overeenkomstige onderdeel.
Natuurlijk is het ook de bedoeling om zelf ook testen te schrijven om te garanderen dat jouw implementatie volledig correct is.
Je mag hiervoor gebruik maken van de meegeleverde `test.h`-header van het framework [acutest](https://github.com/mity/acutest), maar dat is niet verplicht.

- Je mag niet inlezen van `stdin`, tenzij dit expliciet wordt meegegeven als argument (`-i /dev/stdin`).
- Je mag niet uitschrijven naar `stdout`, tenzij dit expliciet wordt meegegeven als argument (`-o /dev/stdout`).
- Je mag niet uitschrijven naar `stderr`, tenzij er iets fout loopt (bv. een ongeldige optie) of als dit expliciet wordt meegegeven als argument (bv. `-v` om verbose modus in te schakelen).
- Je programma moet single threaded zijn en mag dus niet forken.
- De buffer die je gebruikt om ingelezen strings in op te slaan mag niet groter zijn dan de grootte die meegegeven werd met de `-m` optie.
- De kleinste buffergrootte die je zal krijgen is `33554432` (32 MiB), je moet hiermee bestanden kunnen comprimeren tot 10 GiB groot.

We voorzien 50% extra geheugen voor je programma om overhead op te vangen. Wanneer we een buffergrootte van 100MiB opgeven, mag het totaal van jouw programma dus niet meer dan 150MiB geheugen gebruiken.
We zullen je geheugengebruik beperken van de CGroup-functionaliteit met `systemd-run`:

```sh
# Run je programma met een buffergrootte van 100MiB geheugen
# Als je programma meer dan 150MiB geheugen wil alloceren, zal het gestopt worden
systemd-run --scope -p "MemoryMax=$((1024*1024*150))" --user ./opc sort -m "$((1024*1024*100))" -i input.txt -o output.txt
```

Het is toegelaten om extra te optimaliseren door ook een deel van deze 50% te gebruiken voor de ingelezen strings, maar dit maakt het project een stuk complexer.

Als je je het geheugengebruik van je programma in detail wil opvolgen, kan je ook gebruik maken van het `massif` programma van Valgrind:

```sh
valgrind --tool=massif --stacks=yes --massif-out-file=memory.ms -- ./opc
```

De bovenstaande lijn zal het testprogramma (`./huffsort`) uitvoeren met de [Valgrind Massif](https://www.valgrind.org/docs/manual/ms-manual.html) tool, die zal een `memory.ms` bestand genereren dat het geheugengebruik beschrijft.
Je kunt dit bestand gewoon openen met een teksteditor of je kan `ms_print` gebruiken om een ASCII art grafiekje te krijgen dat het geheugengebruik doorheen de tijd toont.
Met de [massif-visualizer](https://github.com/KDE/massif-visualizer) tool kun je een mooiere weergave krijgen (met kleuren en grafieken waar je op kunt klikken).

*Belangrijk bij het meten van geheugen*:

- Compileer je programma met `-g` om debuginformatie aan te zetten zodat deze tools mooi kunnen tonen waar er geheugen gealloceerd is.
- De dumpbestanden van Valgrind Massif gebruiken de notatie `kB` voor 1024 byte, een correctere notatie zou kiB zijn.
- Valgrind heeft ook een [memcheck](https://www.valgrind.org/docs/manual/mc-manual.html) tool waarmee je memory leaks kan opsporen.
- Je programma kan tot 100 keer trager worden tijdens het uitvoeren in `valgrind`.

## Tips

We geven je alvast enkele tips om je op weg te helpen:

- Denk goed na hoe je de optimale orde-bewarende prefix-codering opstelt. Kan je daarvoor het algoritme voor Huffman-codering gebruiken? Je kan ook inspiratie opdoen bij het algoritme om een optimale binaire zoekboom op te stellen.
- Wanneer je met een beperkt geheugen zit (bijvoorbeeld bij extern sorteren), kan je soms de neiging hebben om je buffer zo goed mogelijk te willen vullen. Dit is efficiënter, maar kan zeer complex worden. Probeer eerst een simpele implementatie te schrijven die bijvoorbeeld altijd minstens de helft van je buffer gebruikt, die kan je achteraf nog optimaliseren indien nodig.

## Onderzoek

In dit project willen we onderzoeken of het sorteren van deze data volgens een orde-bewarende prefix-codering een goed idee is.
Zijn er gevallen waar de Huffman-code orde-bewarend is?
Als de data al gecomprimeerd is door jouw programma, hoe groot is het voordeel om te sorteren in vergelijking met het sorteren van de ongecomprimeerde data? 

*Tip:* Als je metingen doet, compileer je natuurlijk met zo veel mogelijk optimalisaties aan (compileer met `-O3`)

## Verslag

Schrijf een bondig verslag over je werk met volgende componenten:

- [ ] De ontwerpbeslissingen die je nam voor de implementaties, in het bijzonder het formaat van je gecomprimeerde bestanden.
- [ ] Een voorbeeld van een tekst waarvoor een orde-bewarende Huffman-codering bestaat en een tekst waarvoor zoiets niet bestaat.
- [ ] Een beschrijving van het algoritme dat je hebt gebruikt om de optimale orde-bewarende prefix-codering op te stellen, **met een korte analyse van de tijdscomplexiteit**.
- [ ] Een duidelijk antwoord op de vraag: "Is het sorteren van deze data volgens een orde-bewarende prefix-codering een goed idee?". Staaf dit antwoord met relevante benchmarks.

Maak je verslag niet onnodig lang.

## Indienen

### Directorystructuur

Je indiening moet volgende de structuur hebben:

- `src/` bevat alle broncode

- `tests/` alle testcode (je moet testcode hebben, push geen grote testbestanden
  naar SubGIT, plaats een scriptje dat die bestanden genereert).

- `extra/verslag.pdf` bevat de elektronische versie van je verslag. In deze map
  kun je ook eventueel extra bijlagen plaatsen zoals de broncode van je verslag.

- `sources`: een tekstbestand met de`.h` en `.c` bronbestanden om je programma te compileren, 1 per lijn relatief t.o.v. de `src` directory. Bijvoorbeeld:

      main.c
      huffman/compress.c
      huffman/extract.c
      sort.c

Je mappenstructuur ziet er dus ongeveer zo uit:

    |
    |-- extra/
    |   `-- verslag.pdf
    |-- src/
    |   `-- je broncode
    |-- test/
    |   `-- je testcode
    |-- data/
    |   `-- onze datasets
    |-- sources

Al je bestanden moeten als UTF-8 opgeslagen zijn.

### Compileren

De code zal bij het indienen gecompileerd worden met (ruwweg) onderstaande
opdracht door SubGIT met **GCC 12**.

```sh
gcc -std=c17 -O3 -Wall -Werror -lm $(cat ./sources) -o huffsort
```

De `Dockerfile` en bijhorende bronbestanden die SubGIT gebruikt, kan je vinden in de Git-repository `git@SubGIT.UGent.be:2023-2024/AD3/project-dockerfile`. Je kan de testen van SubGIT lokaal uitvoeren met de volgende commando's in deze repository:

```sh
docker build . -t ad3-project-2023-2024 # hoef je maar één keer te doen
docker run -it --rm --mount type=bind,source={PAD},destination=/submission,readonly ad3-project-2023-2024
```

Waarbij `{PAD}` vervangen dient te worden door het absolute pad naar de hoofdmap
(niet `src`) van je code.

### SubGIT

Het indienen gebeurt via het [SubGIT](https://SubGIT.ugent.be/) platform. Indien je hier nog geen account op hebt, dien je die aan te maken.

#### Repository afhalen

```bash
git clone git@SubGIT.ugent.be:2023-2024/AD3/project/{studentnr} projectAD3
```

Deze repository zal leeg zijn.
Recentere versies van Git zullen automatisch een `main` branch aanmaken, maar op SubGit wordt nog `master` gebruikt.
Pas je lokale repository aan zodat je `master` gebruikt als default branch:

```bash
cd projectAD3
git branch -m master
```

#### Opgave als `upstream` instellen

Je kunt de opgave en data voor het project afhalen door de opgave repository als upstream in te stellen met volgende commando's in de `projectAD3` map:

```bash
git remote add upstream git@SubGIT.UGent.be:2023-2024/AD3/project-assignment
git pull upstream master
```

(Je kunt niet pushen naar de upstream, alleen de lesgevers kunnen dat.)

#### Feedback

Als je pusht naar SubGIT, zul je in je terminal te zien krijgen of je code
voldoet aan de minimumvereisten. In dat geval krijg je bij het pushen de melding
dat het pushen geslaagd is:

```
remote: Acceptable submission
```

Je kunt geen code pushen naar de `master` branch als die niet compileert of niet
aan de minimale vereisten voldoet. Je kunt echter wel pushen naar andere
branches en daar zal je push wel aanvaard worden.

#### De `master` branch

De `master` branch op SubGIT stelt jouw indiening voor. Je kunt voor de deadline
zoveel pushen als je wilt. Zorg ervoor dat je voor de deadline zeker je finale
versie naar de **`master`** branch hebt gepusht, want enkel die zal verbeterd worden.

#### Controleren of je zeker goed hebt ingediend

Je kunt jouw indiening bekijken door jouw repository nog eens te clonen in een andere map

```bash
cd eenAndereMap
git clone git@SubGIT.ugent.be:2023-2024/AD3/project/{studentnr} projectAD3Controle
```

### Deadlines en belangrijke data

Zorg ervoor dat je een tussentijdse versie hebt gepusht naar een branch (niet noodzakelijk `master`, moet zelfs niet compileren) op SubGIT voor **woensdag 2023-11-22 om 17:00:00**.
De versie die je hier indient, zal niet beoordeeld worden, we vragen je dit te doen om er zeker van te zijn dat iedereen kan indienen.
Als je niets pusht voor deze deadline verlies je al je punten voor het project.

De code die op **woensdag 2023-12-06 om 17:00:00** op de `master` branch staat, is je finale code-indiening.
Enkel code die op dat moment op de `master` branch staat, wordt verbeterd.
**Als er geen code op je `master` branch staat, krijg je nul op vier voor het project.**
Je kunt na deze deadline nog steeds pushen om wijzigingen aan te brengen aan het verslag en je benchmarks.

Na **woensdag 2023-12-13 om 17:00:00** kun je ook helemaal geen wijzigingen meer aanbrengen aan je repo.
Het verslag dat aanwezig is als pdf op de `master` branch in `extra/verslag.pdf` is je finaal verslag.

## Algemene richtlijnen

**Belangrijk:** het gebruik van AI-assistenten zoals GitHub CoPilot, ChatGPT, ... is niet toegelaten voor dit project.
Omdat dit een project is dat meetelt voor je eindscore van dit vak, zitten we hier in een "examen"-situatie waarbij je individueel een oplossing moet maken.
Het gebruik van dit soort assistenten zal beschouwd worden als **fraude**. 

- Schrijf efficiënte code, maar ga niet over-optimaliseren: **geef de voorkeur
  aan elegante, goed leesbare code**. Kies zinvolle namen voor methoden en
  variabelen en voorzie voldoende commentaar. Let op geheugenhygiëne: ga actief
  op zoek naar geheugenlekken en alloceer niet onnodig veel geheugen.
- Op SubGIT staat een Dockerfile waarmee je een Linux container kunt bouwen die
  jouw code compileert en minimale testen uitvoert. Als alle testen slagen zal 
  dit programma `Acceptable submission` uitschrijven als laatste regel en 
  stoppen met exit code 0. Code die hier niet aan voldoet, zal geweigerd worden
  door SubGIT en levert geen punten op (0/4 voor het project).
- Het project wordt gequoteerd op **4** van de 20 te behalen punten voor dit
  vak, en deze punten worden ongewijzigd overgenomen naar de tweede
  examenperiode.
- Projecten die ons niet (via de `master` branch op SubGIT) bereiken voor de
  deadline worden niet meer verbeterd: dit betekent het verlies van alle te
  behalen punten voor het project.
- Dit is een individueel project en dient dus door jou persoonlijk gemaakt te
  worden. Het is uiteraard toegestaan om ideeën
  uit te wisselen, maar **het is verboden code uit te wisselen**,
  op welke manier dan ook. Het overnemen van code beschouwen we als fraude (van
  **beide** betrokken partijen) en zal in overeenstemming met het
  examenreglement behandeld worden. Op het internet zullen ongetwijfeld ook
  (delen van) implementaties te vinden zijn. Het overnemen of aanpassen van
  dergelijke code is echter **niet toegelaten** en wordt gezien als fraude. (Als
  je je code op GitHub/Bitbucket/sourcehut/Gitlab/… plaatst moet je die privaat
  houden).
- Ook het gebruik van AI-assistenten zoals ChatGPT, GitHub CoPilot, ...
  worden beschouwd als fraude: zij zijn getraind op bestaande code en produceren
  bestaande code vaak zonder dat je dat wil.
- Essentiële vragen worden **niet** meer beantwoord tijdens de week voor
  de laatste deadline.

## Vragen

Als je vragen hebt over de opgave of problemen ondervindt, dan kun je je vraag
stellen tijdens het practicum op dinsdagvoormiddag. Contacteer je ons per mail,
stuur dan ook je studentennummer en de branch waarop de code met het probleem
zit mee. Zo kunnen we onmiddellijk je code pullen. Stuur geen screenshots van
code of foutboodschappen.

