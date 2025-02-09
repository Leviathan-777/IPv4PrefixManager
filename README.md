# IPv4PrefixManager

**Program spełnia poniższe zadania:**

-Funkcja add pozwalająca na dodanie prefiksów

-Funckja del pozwalająca na usunięcie prefiksów

-Funkcja check pozwalająca na sprawdzenie czy dany adres znajduję się w zbiorze prefiksów

-Funkcja test sprawdzająca działanie powyższych funckji

**Kompilacja/Uruchomienie:**
1. gcc src\IPv4Manager.c -o out\IPv4Manager
2. out\IPv4Manager

**Repozytorium posiada również konfiugarcję Dockera więc może program może być skompilowany i uruchomiony również w Containerze:**
1. docker build -t ipv4_manager .
2. docker run -it ipv4_manager
