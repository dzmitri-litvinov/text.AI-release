document.addEventListener("DOMContentLoaded", function () {
  const sendButton = document.getElementById("send"); // button send
  const openButton = document.getElementById("open") // button open
  const infoDatabase = document.getElementById("info") // info database
  const request = new XMLHttpRequest();
  const regexp = /[^(\d+(.\d+)?)]/g;

  sendButton.addEventListener("click", function () {
    let payload =
        "name=" + encodeURIComponent(document.getElementById("value").value);

    request.open("POST", "http://34.125.1.13:8080/cgi-bin/script.cgi", true);

    request.addEventListener("readystatechange", function () {
      const answerNumber = 100 - parseInt(request.responseText.replace(regexp, ""));
      console.log(answerNumber);
      document.querySelector("#opacity").classList.add("opacity");
      document.querySelector("#result").innerHTML = `${answerNumber}% unique`;
    });

    request.setRequestHeader(
        "Content-Type",
        "application/x-www-form-urlencoded"
    );
    request.send(payload);
  });

  openButton.addEventListener("click", function () {
    // проверить работает ли этот запрос на получение из базы данных текста
    request.open("GET", "http://34.125.1.13:8080/cgi-bin/script.cgi");
    request.send();

    request.onload = function () {
      console.log(this.responseText);
      infoDatabase.innerHTML = this.responseText;
      openButton.disabled = true;
    };
  })
});
