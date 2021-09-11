document.addEventListener("DOMContentLoaded", function () {
  const sendButton = document.getElementById("send"); // button send
  const openButton = document.getElementById("open") // button open
  const infoDatabase = document.getElementById("info") // info database
  
  const regexp = /[^(\d+(.\d+)?)]/g;

  sendButton.addEventListener("click", function () {
    let payload =
        "name=" + encodeURIComponent(document.getElementById("value").value);
        const request = new XMLHttpRequest();
    request.open("POST", "http://localhost:4040/cgi-bin/script.cgi", true);

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
    const request1 = new XMLHttpRequest();
    // проверить работает ли этот запрос на получение из базы данных текста
    request1.open("GET", "http://localhost:4040/cgi-bin/text.cgi");
    request1.send();

    request1.onload = function () {
      console.log(request1.responseText);
      infoDatabase.innerHTML = request1.responseText;
      openButton.disabled = true;
    };
  })
});
