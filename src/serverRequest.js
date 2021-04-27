document.addEventListener("DOMContentLoaded", function () {
  const sendButton = document.getElementById("send"); // button send
  const regexp = /[^(\d+(.\d+)?)]/g;
  sendButton.addEventListener("click", function () {
    let payload =
      "name=" + encodeURIComponent(document.getElementById("value").value);

    const request = new XMLHttpRequest();
    request.open("POST", "http://34.125.1.13:8080/cgi-bin/script.cgi", true);

    request.addEventListener("readystatechange", function () {
      console.log(parseInt(request.responseText.replace(regexp, "")));
      document.querySelector("#img").classList.add("opacity");
      document.querySelector("#result").innerHTML = `${parseInt(
        request.responseText.replace(regexp, "")
      )}% unique`;
    });

    request.setRequestHeader(
      "Content-Type",
      "application/x-www-form-urlencoded"
    );
    request.send(payload);
  });
});
