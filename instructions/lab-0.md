# Instalacija programskog okruženja

**NAPOMENA:** Za instalaciju programskih alata potrebno je imati administratorske ovlasti na računalu

## [Python](https://www.python.org)

PlatformIO IDE se temelji na PlatformIO Core koji je napisan u Python programskom jeziku. Potrebno je skinuti i instalirati najnoviju verziju [Python 2.7.x](https://www.python.org/downloads/). **NAPOMENA:** Vodite računa da je prilikom instalacije potrebno kliknuti na `Add python.exe to Path` u "Customize" koraku, jer u protivnom `python` komanda neće biti dostupna u Command Promptu.

![python-installer-add-path](https://cloud.githubusercontent.com/assets/8695815/23476700/f1482f08-febb-11e6-8786-ac2bbf17cb0b.png)


- Nakon uspješne instalacije otvorite Command Prompt te upišite naredbu `python` čime testirate uspješnost instalacije:

```
C:\Users\A507>python
```

## [Clang](http://clang.llvm.org/)

PlatformIO IDE upotrebljava [Clang](http://clang.llvm.org/). Da biste provjerili dostupnost u vašem OS-u, u Command Promptu upišite sljedeću naredbu: `clang --version`.
Ako Clang nije instaliran na vašem računalu, skinite ga i instalirajte. Pri tome vodite računa da ste označili `Add LLVM to the system PATH` prilikom instalacije.

![clang-installer-add-path](https://cloud.githubusercontent.com/assets/8695815/23476985/c9aaf178-febc-11e6-83a9-d09d80c07081.png)

## [PlatformIO](http://platformio.org/)

Skinite i instalirajte [Visual Studio Code](https://code.visualstudio.com/) editor.
Otvorite Extensions te Instalirajte `platformio-ide` package (auto installer) kao što je prikazano na slici ispod.

![ide-atom-platformio-install](https://user-images.githubusercontent.com/8695815/76229546-ada67980-6222-11ea-8dd0-818984d19f1d.png)


## [Git klijent za Windows](https://git-for-windows.github.io/)

Git klijent `Git Bash` omogućuje jednostavno preuzimanje koda sa GitHub repozitorija. Testirajte instalaciju tako da pokušate klonirati repozitorij `https://github.com/toperkov/WiSe-2018-19.git`. U Git Bash komandnom prozoru izvršite sljedeće:

```bash
# U osobnom direktoriju (npr. `C:/Users/A507/Toni`) izvršite
$ git clone https://github.com/toperkov/WiSe-2019-20.git labs-intro
$ cd labs-intro
$ ls
```