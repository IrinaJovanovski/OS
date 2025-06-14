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
[Projektni zadatak 2024. v1.0.pdf](https://github.com/user-attachments/files/20256115/Projektni.zadatak.2024.v1.0.pdf) \
\
----------------------------------------------------------------------------------------------------------------------

The goal of this project is to implement a small but fully functional kernel of an operating system that supports threads (multithreaded operating system) with time sharing. In the following text, this system will be briefly referred to as the kernel.

Within this project, a memory allocator and thread management need to be implemented. The kernel should provide the concept of threads, semaphores, and support for time sharing, as well as asynchronous context switching and preemption triggered by timer and keyboard interrupts. 

The kernel should be implemented as a "library," so that the user program (application) and the kernel share the same address space, meaning they represent a statically linked unique program preloaded into the computer's operational memory. Concurrent processes created within the application will actually be "lightweight" processes, i.e., threads running within that program. This configuration is characteristic of embedded systems, which do not execute arbitrary programs that are loaded and run on user demand, but rather execute only the program (along with the operating system) that is already embedded in the target hardware. 

The kernel is implemented for the RISC-V processor architecture and a school computer with this processor. An assembler for this processor and the C/C++ programming language can be used for the implementation. The implemented kernel will run in a virtual environment – an emulator for the RISC-V processor. 
