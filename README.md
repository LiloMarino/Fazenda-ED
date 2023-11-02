# FarmKraft
Este trabalho simula uma fazenda por meio da leitura de arquivos .geo e .qry seguindo os seguintes comandos dentro de seus arquivos e gera a fazenda em seu estado incial (após a execução do .geo) 
e seu estado final (após a execução do .qry) e também gera um txt de relatório acerca dos comandos .qry e também gera um .dot retratando o estado final da árvore radial
### Arquivos Geo

| Comando | Parâmetros | Descrição |
|--------|--------|--------|
| c | i x y r corb corp | Desenhar círculo corb é a cor da borda e corp é a cor do preenchimento|
| r | i x y w h corb corp | Desenhar retângulo: w é a largura do retângulo e h, a altura. corb é a cor da borda e corp é a cor do preenchimento|
| l | i x1 y1 x2 y2 cor | Desenhar linha com extremidades nos pontos (x1,y1) e (x2,y2), com a cor especificada.|
| ts | fFamily fWeight fSize | Muda o estilo dos textos (comando t) subsequentes. font family: sans (sans-serif), serif, cursive; font weight ( n: normal, b: bold, b+: bolder, l: | lighter)|
| t | i x y corb corp a txto | Desenha o texto txto nas coordenadas (x,y) e com a cores indicadas. corb é a cor da borda e corp é a cor do preenchimento. O parâmetro a determina a posição da âncora do texto: i, no início; m, no meio, f, no fim. O texto txto é o último parâmetro do comando. Pode incluir espaços em branco e se estende até o final da linha.| 
### Arquivos Qry
| Comando | Parâmetros | Descrição |
|--------|--------|--------|
| cl  | i | Informa que o retângulo de código i é uma colheitadeira. | 
| hvt | i p ( n / s / l / o ) | Colheitadeira de código i move-se p passos em uma dada direção (n: norte, s: sul, l: leste, o: oeste). Elementos colhidos são contabilizados e removidos do terreno. TXT: reportar os atributos da figura i, a posição original e a posição final. Reportar os atributos dos elementos colhidos e a contabilidade parcial (i.e., dessa colheita) SVG: desenhar um retângulo de bordas vermelhas pontilhadas indicando a região colhida | 
|mv | i dx dy | Move figura/texto de identificador i em dx (no eixo x) e dy (no eixo y). dx e dy podem ser negativos TXT: reportar os atributos da figura i, a posição original e a posição final. |
| ct | x y w h r | Pragas de raio r são uniformemente distribuídas na região retangular (x,y,w,h) danificando cumulativamente as hortaliças atingidas na mesma proporção da área da praga. Caso a hortaliça tenha mais de 75% de sua área atingida, ela é eliminada. TXT: reportar atributos das hortaliças atingidas (não esquecer grau de dano), assinalando as que foram eliminadas. SVG: colocar um retângulo de bordas vermelhas pontilhadas na região (x,y,w,h), um círculo de borda vermelha raio r em (x,y). Marcar com um x vermelho a posição da figura eliminada |
| cr | x y w h r | Similar a ct. Cura as hortaliças na mesma proporção das “goticulas” de raio r do defensivo. TXT: reportar atributos das hortaliças atingidas (não esquecer grau de dano) SVG: colocar um retângulo de bordas vermelhas pontilhadas na região (x,y,w,h), um círculo de borda amarela e raio r em (x,y). |
| ad | x y w h r | Similar a ct. Sempre que a área integral da hortaliça for atingida pelo adubo, a produtividade4 da hortaliça é incrementada em 10%. TXT: reportar atributos das hortaliças atingidas. SVG: colocar um retângulo de bordas vermelhas pontilhadas na região (x,y,w,h), um círculo de borda verde e raio r em (x,y). |
| st | x y w h f dx dy j | Dispersa as sementes das hortaliças que estão dentro da região (x,y,w,h), a um deslocamento de dx, dy, com fator f, com identificadores a partir de j. TXT: reportar id e atributos dos elementos atingidos; id e atributos dos clones; reportar os atributos dos clones. SVG: colocar um retângulo de bordas vermelhas pontilhadas na região (x,y,w,h) e outro semelhante na região deslocada. Colocar um pequeno círculo com preenchimento vermelho em (x,y) |
| d? | i | Dados da figura de identificador i. TXT: reportar todos os atributos da figura de código i. |
| c? ||TXT: reporta os atributos de todas as colheitadeiras.|
### Contabilidade da Colheita
A contabilidade da colheita é baseada na seguinte tabela

| Produto | Peso |
|--------|--------|
|Abóbora|2kg|
|Morango|20g|
|Repolho|1kg|
|Cebola|200g|
|Cenoura|70g|
|Mato(linha) |10g por unidade de comprimento|
|Mato(texto) |15g|
