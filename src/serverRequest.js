document.addEventListener("DOMContentLoaded", function () {
  const textAreaCheckNode = document.getElementById('value'); // textarea for send button
  const textAreaInfoNode = document.getElementById("info") // textarea for open database
  const sendButton = document.getElementById("send"); // button send
  const openButton = document.getElementById("open") // button open
  const WORDS_NUMBER = 2;
  
  const regexp = /[^(\d+(.\d+)?)]/g;

  textAreaCheckNode.addEventListener("keyup", (e) => {
    let arrayWords = e.target.value.split(" ");
    let counterWords = arrayWords.length;

    if (arrayWords[WORDS_NUMBER] !== "") {
      if (counterWords > WORDS_NUMBER) {
        sendButton.disabled = false;
        sendButton.title = "";
      } else {
        sendButton.disabled = true;
        sendButton.title = "Enter at least 3 words!";
      }
    }

    if (counterWords > WORDS_NUMBER && e.code === "Enter") {
      sendButton.disabled = false;
      sendButton.title = "";
      sendButton.click();
    }
  });

  sendButton.addEventListener("click", function () {
    let payload =
        "name=" + encodeURIComponent(document.getElementById("value").value);
        const request = new XMLHttpRequest();
    request.open("POST", "http://3.15.205.38:8080/cgi-bin/script.cgi", true);

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

  openButton.addEventListener("click",  function () {
    const request1 = new XMLHttpRequest();
    request1.open("GET", "http://3.15.205.38:8080/cgi-bin/text.cgi");
    request1.send();

    request1.onload = function () {
      console.log(request1.responseText);
      textAreaInfoNode.innerHTML = request1.responseText;
      textAreaInfoNode.disabled = true;
      textAreaInfoNode.style.color = 'black';
      openButton.disabled = true;
    };
  })
});
