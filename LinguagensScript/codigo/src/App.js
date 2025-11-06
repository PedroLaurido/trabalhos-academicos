import "./App.css";
import "./components/MainCss/main.css";
import logodiogo from "./components/imagens/fish.png";
import logofabio from "./components/imagens/soccer.png";
import logopedro from "./components/imagens/gym.png";
import React, { useState, useEffect } from "react";

function App() {
  const [turno, setTurno] = useState("X");
  const [quadrados, setQuadrados] = useState(
    Array(9)
      .fill(null)
      .map(() => Array(9).fill(null))
  );
  const [tabuleirosPequenos, setTabuleirosPequenos] = useState(
    Array(9)
      .fill(null)
      .map(() => Array(9).fill(null))
  );
  const [vencedores, setVencedores] = useState([]);
  const [tempoJogador1, setTempoJogador1] = useState(300); // 5 minutos
  const [tempoJogador2, setTempoJogador2] = useState(300); // 5 minutos
  const [intervalId, setIntervalId] = useState(null);
  const [gameStarted, setGameStarted] = useState(false);
  const [tempoEsgotado, setTempoEsgotado] = useState(false);
  const [name1, setName1] = useState("Player1");
  const [name2, setName2] = useState("Player2");

  function VerificaWin(tabuleiro) {
    const vitorias = [
      [0, 1, 2],
      [3, 4, 5],
      [6, 7, 8],
      [0, 3, 6],
      [1, 4, 7],
      [2, 5, 8],
      [0, 4, 8],
      [2, 4, 6],
    ];

    for (let i = 0; i < vitorias.length; i++) {
      const [a, b, c] = vitorias[i];
      if (
        tabuleiro[a] &&
        tabuleiro[a] === tabuleiro[b] &&
        tabuleiro[a] === tabuleiro[c]
      ) {
        return tabuleiro[a];
      }
    }

    return null;
  }

  useEffect(() => {
    if (intervalId) {
      clearInterval(intervalId);
    }

    const newIntervalId = setInterval(() => {
      if (turno === "X") {
        setTempoJogador1((tempo) => (tempo > 0 ? tempo - 1 : 0));
      } else {
        setTempoJogador2((tempo) => (tempo > 0 ? tempo - 1 : 0));
      }
    }, 1000);

    setIntervalId(newIntervalId);

    // Verifica se existe um vencedor final
    const vencedorFinal = VerificaWin(vencedores);
    if (vencedorFinal) {
      clearInterval(newIntervalId); // Limpa o intervalo para parar de decrementar o tempo
    }

    // Verifica se o tempo esgotou para o jogador atual
    if (
      (turno === "X" && tempoJogador1 <= 0) ||
      (turno === "O" && tempoJogador2 <= 0)
    ) {
      setTempoEsgotado(true);
      clearInterval(newIntervalId); // Limpa o intervalo para parar de decrementar o tempo
      window.location.href = "/";
    } else {
      setTempoEsgotado(false);
    }

    return () => {
      if (newIntervalId) {
        clearInterval(newIntervalId);
      }
    };
  }, [turno, gameStarted, tempoJogador1, tempoJogador2, vencedores]);

  function VerificaVitoriaTabuleiro(ntab) {
    const tabuleiroAtual = tabuleirosPequenos[ntab];
    const vencedorTabuleiro = VerificaWin(tabuleiroAtual);
    if (vencedorTabuleiro) {
      const novosVencedores = [...vencedores];
      novosVencedores[ntab] = vencedorTabuleiro;
      setVencedores(novosVencedores);
    }
  }

  function VerificaVitoriaTabuleiroPrincipal() {
    const vitorias = [
      // linhas
      [0, 1, 2],
      [3, 4, 5],
      [6, 7, 8],
      // colunas
      [0, 3, 6],
      [1, 4, 7],
      [2, 5, 8],
      // diagonais
      [0, 4, 8],
      [2, 4, 6],
    ];

    for (let i = 0; i < vitorias.length; i++) {
      const [a, b, c] = vitorias[i];
      const vencedoresNestesTabuleiros = [
        vencedores[a],
        vencedores[b],
        vencedores[c],
      ];
      if (
        vencedoresNestesTabuleiros.every(
          (vencedor) =>
            vencedor !== null && vencedor === vencedoresNestesTabuleiros[0]
        )
      ) {
        if (
          vencedoresNestesTabuleiros[0] === "X" ||
          vencedoresNestesTabuleiros[0] === "O"
        ) {
        } else {
          return null;
        }
      }
    }
  }

  function Clique(num, ntab) {
    if (vencedores[ntab]) {
      return;
    }

    const tabuleiroAtual = tabuleirosPequenos[ntab];
    const novoTabuleiro = [...tabuleiroAtual];

    if (novoTabuleiro[num] !== null) {
      return;
    }

    novoTabuleiro[num] = turno;
    const novosTabuleiros = [...tabuleirosPequenos];
    novosTabuleiros[ntab] = novoTabuleiro;
    setTabuleirosPequenos(novosTabuleiros);

    const vencedorTabuleiro = VerificaWin(novoTabuleiro);
    if (vencedorTabuleiro) {
      const novosVencedores = [...vencedores];
      novosVencedores[ntab] = vencedorTabuleiro;
      setVencedores(novosVencedores);
    }

    setTurno(turno === "X" ? "O" : "X");

    if (VerificaWin(tabuleiroAtual)) {
      const novosVencedores = [...vencedores];
      novosVencedores[ntab] = turno;
      setVencedores(novosVencedores);
      return;
    }

    VerificaVitoriaTabuleiro(ntab);
  }

  function iniciarJogo() {
    setGameStarted(true); // Define o estado do jogo como iniciado
  }

  function reiniciarJogo() {
    setTurno("X");
    setQuadrados(Array(9).fill(Array(9).fill(null)));
    setTabuleirosPequenos(Array(9).fill(Array(9).fill(null)));
    setVencedores([]);
    setTempoJogador1(300);
    setTempoJogador2(300);
  }

  function creditos() {
    document.getElementById("cointainer").style.display = "none";

    document.getElementById("creditos").style.display = "block";
  }

  function menuPrincipal() {
    document.getElementById("creditos").style.display = "none";

    document.getElementById("creditos").style.display = "none";

    document.getElementById("cointainer").style.display = "block";
  }

  const handleName1 = (e) => {
    setName1(e.target.value);
    console.log(name1);
  };

  const handleName2 = (e) => {
    setName2(e.target.value);
    console.log(name2);
  };

  function Celula({ num, ntab }) {
    const tabuleiroAtual = tabuleirosPequenos[ntab];
    const valorCelula = tabuleiroAtual[num];
    return <td onClick={() => Clique(num, ntab)}>{valorCelula}</td>;
  }

  function TabuleiroPequeno({ ntab }) {
    // Verificar se o tabuleiro atual tem um vencedor
    const temVencedor = vencedores[ntab];
    const jogadorVencedor = temVencedor === "X" ? "x" : "o";
    // Definir a classe CSS para o tabuleiro atual
    let classeCSS;
    if (VerificaVitoriaTabuleiroPrincipal() || tempoJogador1 <= 0) {
      if (temVencedor) {
        classeCSS = `tabuleiro-pequeno-vencedor-${jogadorVencedor}`;
      } else {
        classeCSS = "jogo_enc_tabuleiro-pequeno";
      }
    } else {
      classeCSS = temVencedor
        ? `tabuleiro-pequeno-vencedor-${jogadorVencedor}`
        : "tabuleiro-pequeno";
    }

    return (
      <table className={classeCSS}>
        <tbody>
          <tr>
            <Celula num={0} ntab={ntab} />
            <Celula num={1} ntab={ntab} />
            <Celula num={2} ntab={ntab} />
          </tr>
          <tr>
            <Celula num={3} ntab={ntab} />
            <Celula num={4} ntab={ntab} />
            <Celula num={5} ntab={ntab} />
          </tr>
          <tr>
            <Celula num={6} ntab={ntab} />
            <Celula num={7} ntab={ntab} />
            <Celula num={8} ntab={ntab} />
          </tr>
        </tbody>
      </table>
    );
  }

  function TabelaPrincipal() {
    let class_tab_princ = "tabela-principal";
    const vencedorPrincipal = VerificaWin(vencedores);
    if (vencedorPrincipal) {
      return (
        <>
          <p>{`${vencedorPrincipal[0]} venceu o ULTIMATE TIC TAC TOE!`}</p>
        </>
      );
    } else {
      if (VerificaVitoriaTabuleiroPrincipal() || tempoJogador1 <= 0) {
        class_tab_princ = "tabela-principal_encerrado";
      }

      return (
        <table className={class_tab_princ}>
          <tbody>
            <tr>
              <td>
                <TabuleiroPequeno ntab={0} />
              </td>
              <td>
                <TabuleiroPequeno ntab={1} />
              </td>
              <td>
                <TabuleiroPequeno ntab={2} />
              </td>
            </tr>
            <tr>
              <td>
                <TabuleiroPequeno ntab={3} />
              </td>
              <td>
                <TabuleiroPequeno ntab={4} />
              </td>
              <td>
                <TabuleiroPequeno ntab={5} />
              </td>
            </tr>
            <tr>
              <td>
                <TabuleiroPequeno ntab={6} />
              </td>
              <td>
                <TabuleiroPequeno ntab={7} />
              </td>
              <td>
                <TabuleiroPequeno ntab={8} />
              </td>
            </tr>
          </tbody>
        </table>
      );
    }
  }
  return (
    <div>
      <div className="container" id="cointainer">
        {gameStarted ? (
          <>
            {gameStarted &&
              !VerificaVitoriaTabuleiroPrincipal() &&
              !tempoEsgotado}
            <p className="">
              Tempo Jogador {name1}: {tempoJogador1}
            </p>
            <p>
              Tempo Jogador {name2}: {tempoJogador2}
            </p>
            <TabelaPrincipal />
            {VerificaVitoriaTabuleiroPrincipal()}
            <button id="play-again" onClick={reiniciarJogo}>
              Reiniciar Jogo
            </button>
            <br></br>
          </>
        ) : (
          <>
            <div id="inputs">
              <input
                id="jogador1"
                type="text"
                placeholder="Jogador1"
                onChange={handleName1}
              />
              <input
                id="jogador2"
                type="text"
                placeholder="Jogador2"
                onChange={handleName2}
              />
            </div>

            <button id="jogadorvsjogador" onClick={iniciarJogo}>
              Jogador VS Jogador
            </button>
            <br></br>
            <button id="btn_creditos" onClick={creditos}>
              Creditos
            </button>
          </>
        )}
      </div>

      <div id="creditos" style={{ display: "none" }}>
        <table id="tabela_creditos">
          <tr>
            <th>Icon</th>
            <th>Nome</th>
            <th>Número</th>
          </tr>
          <tr>
            <td>
              <img
                src={logodiogo}
                alt="Ícone 1"
                style={{
                  width: "50px",
                  height: "50px",
                  padding: "5px",
                }}
              />
            </td>
            <td>Diogo Oliveira</td>
            <td>2021146037</td>
          </tr>
          <tr>
            <td>
              <img
                src={logofabio}
                alt="Ícone 1"
                style={{ width: "50px", height: "50px", padding: "5px" }}
              />
            </td>
            <td>Fábio Pereira</td>
            <td>2021129697</td>
          </tr>
          <tr>
            <td>
              <img
                src={logopedro}
                alt="Ícone 1"
                style={{ width: "50px", height: "50px", padding: "5px" }}
              />
            </td>
            <td>Pedro Pereira</td>
            <td>2021130905</td>
          </tr>
        </table>

        <button id="voltar" onClick={menuPrincipal}>
          Voltar ao Menu Principal
        </button>
      </div>
    </div>
  );
}

export default App;
