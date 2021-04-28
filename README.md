# Uniqueness of the text

This is app allow you check uniqueness of the entered text in the next stories: .
You have to write _three or more words_ in textarea and click on button check and in result you will get percent uniqueness your text.
If result will **0 percent** - entered text is an unoriginal and **anti plagiarism**!

## Visuals

![ScreenShot](https://i.postimg.cc/LsmPRJmc/Screenshot-1.png)

## Built With

- **C++**
- **Javascript**
- **CSS**
- **HTML**
- **Docker**
- **Makefile**

## Getting Started

Go the following [link](http://34.125.1.13:8080/).

## Installation

```
Install Docker Engine Community on Ubuntu 18.04 LTS on your virtual machine and connect it by SSH.

Make git clone this repo.

Type cd reponame to go to the repository directory.

Open and replace in line 9 current ip address to external ip address in serverRequest.js file (src/serverRequest.js).

Push this change to your git.

Then, pull the change from the git using the command git pull in the terminal.

Then type the following commands to build and run the app:

sudo docker build -t app:1.0 .
sudo docker run -d -p 8080:80 app:1.0

Note: if port 8080 is already allocated then write another port 8081:80 in serverRequest.js and back to point 3.6.

Type http: <external_ip>:8080/ in your browser for checking your app
```

## Support

*Contact with us in **_github_**:*

[Artem](https://github.com/PWMaestro) and [Ivan](https://github.com/Gerakl22)

## Authors and acknowledgment

Thank you for help mentor [Alex](https://github.com/alexduboisky) and [DevIncubator](https://github.com/DevIncubator)

## License

MIT © Artem and Ivan
