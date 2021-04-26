const selectBodyNode = document.querySelector(".c-select__body");
const selectNode = document.querySelector(".c-select");
let textAreaNode = document.querySelector(".c-antiPlagiarizm__textarea");
const btnSendNode = document.querySelector(".c-antiPlagiarizm__button");
const WORDS_NUMBER = 2;

function selectOption() {
  let text = this.innerText;
  let src = this.getElementsByTagName("img")[0].getAttribute("src");
  let currentText = this.parentNode.parentNode.firstElementChild.getElementsByTagName(
    "span"
  )[0];
  let currentImg = this.parentNode.parentNode.firstElementChild.getElementsByTagName(
    "img"
  )[0];

  currentText.innerText = text;
  currentImg.src = src;
}

Array.from(selectBodyNode.children).forEach((prop) => {
  prop.addEventListener("click", selectOption);
});

selectNode.addEventListener("click", () => {
  selectBodyNode.classList.toggle("is-active");
});

textAreaNode.addEventListener("keyup", (e) => {
  let arrayWords = e.target.value.split(" ");
  let counterWords = arrayWords.length;

  if (arrayWords[WORDS_NUMBER] !== "") {
    if (counterWords > WORDS_NUMBER) {
      btnSendNode.disabled = false;
      btnSendNode.title = "";
    } else {
      btnSendNode.disabled = true;
      btnSendNode.title = "Enter at least 3 words!";
    }
  }

  if (counterWords > WORDS_NUMBER && e.code === "Enter") {
    btnSendNode.disabled = false;
    btnSendNode.title = "";
    btnSendNode.click();
  }
});
