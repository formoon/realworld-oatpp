# Background
Realworld is ["The Mother of all demo apps"](https://codebase.show/projects/realworld).  
This project specifies a precise API description. Thus there are a large number of independent front-end implementations and back-end implementations. Any front-end can working with another set of back-ends.  
On the other hand, Realworld defines a full-featured forum system. In order to achieve this exquisite small goal, a lot of practical technology needs to be applied. If the front-end and back-end solutions achieve this goal, and it is easy to verify the development potential in other Internet application topics.  

# C++ - OAT++
This back-end implementation uses C++ and OAT++ Web framework. The database system uses PostgreSQL through oatpp-postgres ORM.  
Compared with other development languages, Web development is indeed not a good choice for C++.  
With OAT++, this process is much simpler, but it is still tedious and difficult.  
But I like C++, that's enough.

# Prerequisite
In macOS:  
```bash
brew install libpq nlohmann-json 
```
Ubuntu:
```bash
sudo apt install zlib1g-dev libssl-dev nlohmann-json3-dev libpq postgresql-server-dev-12 -y
```

Centos:  
```bash
sudo yum install -y json-devel *atomic* zlib-devel libpq-devel postgresql-server-devel
```

# Setting PostgreSQL
Use PSQL client Setting database and user:  
```sql
CREATE USER realworld WITH PASSWORD 'realworld';
CREATE DATABASE realworld;
GRANT ALL PRIVILEGES ON DATABASE realworld TO realworld;
```
And all tables:
```sql
CREATE TABLE users (
  id SERIAL PRIMARY KEY,
  username TEXT NOT NULL UNIQUE,
  email TEXT NOT NULL UNIQUE,
  bio TEXT,
  image TEXT,
  hash TEXT NOT NULL
);

CREATE TABLE articles (
  id SERIAL PRIMARY KEY,
  slug TEXT NOT NULL UNIQUE,
  title TEXT NOT NULL,
  description TEXT NOT NULL,
  body TEXT NOT NULL,
  author INTEGER NOT NULL REFERENCES users ON DELETE CASCADE,
  tag_list TEXT[] NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
  updated_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
  favorites_count INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE favorites (
       "user" INTEGER REFERENCES users ON DELETE CASCADE,
       article INTEGER REFERENCES articles ON DELETE CASCADE,
       PRIMARY KEY ("user", article)
);

CREATE TABLE follows (
       follower INTEGER REFERENCES users ON DELETE CASCADE,
       followed INTEGER REFERENCES users ON DELETE CASCADE,
       CHECK (follower != followed),
       PRIMARY KEY(follower, followed)
);

CREATE TABLE comments (
       id SERIAL PRIMARY KEY,
       body TEXT NOT NULL,
       article INTEGER NOT NULL REFERENCES articles ON DELETE CASCADE,
       author INTEGER NOT NULL REFERENCES users ON DELETE CASCADE,
       created_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW(),
       updated_at TIMESTAMP WITH TIME ZONE NOT NULL DEFAULT NOW()
);
```

## Compile and install oatpp framework
* oatpp
* oatpp-zlib
* oatpp-postgres

`\utility\install-oatpp-modules.sh` script can help you do this in Ubuntu, Other system can also used as reference.  

Because cpp-jwt module code need std-c++-14, so g++ version need later than 8, macOS clang works well.  

## clone code
```bash
git clone https://github.com/formoon/realworld-oatpp
```

## setting
Create a setting file named: `config.json` in the root of project, content like this:
```js
{
    "thread": {
        "dataThread": 4,
        "ioThread": 1,
        "timerThread": 1
    },
    "server":{
        "address": "0.0.0.0",
        "port": 8002,
        "secretKey": "8Xui8SN4mI+7egV/9dlfYYLGQJeEx4+DwmSQLwDVXJg=",
        "publicMode": true,
        "upAndDownPath": "./download/",
        "staticFilePath": "./static/"
    },
    "postgres":{
        "url": "postgres://realworld:realworld@127.0.0.1:6551/realworld",
        "pool": 10,
        "ttl": 5
    }
}
```

## compile and run
```bash
cd realworld-oatpp
mkdir build
cd build
cmake ..
make -j4
cd ..
# cp config.json.txt config.json
./build/realworld-oatpp
```

## Demo address
<http://39.105.37.153:8002>  
(This is not a permenant server, so may not exist long)


# others
Upload function and Static file holder need front-end cooperate, suggest use [my vue version](https://github.com/formoon/vue-realworld-example-app), it's a revisionary from [Vue2 version RealWorld Frontend](https://github.com/gothinkster/vue-realworld-example-app)





