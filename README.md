# OS
**Operating Systems 1 project - small, functional OS kernel** \
Cilj ovog projekta jeste realizacija malog, ali sasvim funkcionalnog jezgra (engl.
_kernel_) operativnog sistema koji podržava niti (engl. _multithreaded operating system_) sa
deljenjem vremena (engl. _time sharing_). U daljem tekstu, ovakav sistem biće kratko nazivan
samo jezgrom. \
U okviru ovog projekta treba realizovati alokator memorije i upravljanje nitima. Jezgro
treba da obezbedi koncept niti (engl. _thread_), semafora i podršku deljenju vremena (engl. _time
sharing_), kao i asinhronu promenu konteksta i preotimanje (engl. _preemption_) na prekid od
tajmera i od tastature. \
Jezgro treba da bude realizovano kao „_bibliotečno_“, tako da korisnički program
(aplikacija) i samo jezgro dele isti adresni prostor, odnosno da predstavljaju statički povezan
jedinstven program unapred učitan u operativnu memoriju računara. Konkurentni procesi
kreirani unutar aplikacije biće zapravo samo „laki“ procesi, tj. niti (engl. _thread_) pokrenuti
unutar tog programa. Ovakva konfiguracija karakteristična je za ugrađene (engl. _embedded_)
sisteme, koji ne izvršavaju proizvoljne programe koji se učitavaju i izvršavaju na zahtev
korisnika, već izvršavaju samo onaj program (zajedno sa operativnim sistemom) koji je već
ugrađen u ciljni hardver. \
Jezgro se implementira za arhitekturu procesora RISC-V i školskog računara sa ovim
procesorom. Za implementaciju se može koristiti **asembler** za ovaj procesor i jezik **C/C++**.
Implementirano jezgro će se izvršavati u virtuelnom okruženju – emulatoru procesora RISC-V. \
[Projektni zadatak 2024. v1.0.pdf](https://github.com/user-attachments/files/20256115/Projektni.zadatak.2024.v1.0.pdf)
