# StreamLabs Assignment

## Introduction

This project is both a client and server application that communicate through a named pipe and can send different object types and call methods on them.

## Building

This project uses [Hunter](https://github.com/ruslo/hunter) for dependency management. You should create a directory for Hunter cache at the root of this project.

```
# mkdir hunter
```

If you want to put it in another place you should set `HUNTER_ROOT` environment variable.

It's better to create a dedicated folder for build outputs.

```
# mkdir build
# cd build
```

Now build the project with [cmake](https://github.com/Kitware/CMake).

```
# cmake ..
# cmake --build .
```

## Usage

You need to run `server.exe` first and then `client.exe`.

To see how to use the client you can type `help`

```
send TYPE VALUE    send VALUE to server
l                  list the list of stored values on the server
call METHOD        call method on the last object
q                  quit
```
