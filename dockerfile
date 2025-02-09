FROM gcc:latest

WORKDIR /app

COPY . .

RUN gcc src/IPv4Manager.c -o out/IPv4Manager

CMD ["./out/IPv4Manager"]