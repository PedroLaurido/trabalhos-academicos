import React from "react";
import "../MainCss/main.css";
import "../header/header.css";

function Header() {
  const voltar = () => {
    window.location.href = "/";
  };

  return (
    <header>
      <p className="titulo" onClick={voltar}>
        Ultimate TicTacToe
      </p>
      <p className="subtitulo">Linguagens Script</p>
    </header>
  );
}
export default Header;
