import glob
import os
import re

def compare_filenames(filename):
    # Extrai o número do nome do arquivo, removendo a extensão ".svg" e a parte inicial do nome
    number = re.findall(r'frame-(\d+)\.svg', filename)
    return int(number[0]) if number else 0

def generate_html(svg_directory):
    # Verifica se o diretório especificado existe
    if not os.path.isdir(svg_directory):
        print(f"O diretório '{svg_directory}' não existe.")
        return

    # Obtém todos os arquivos .svg na pasta especificada e os ordena alfabeticamente
    svgs = sorted(glob.glob(os.path.join(svg_directory, "*.svg")), key=compare_filenames)

    # Inicializa a string HTML
    html = '<!DOCTYPE html">\n<HTML>\n<HEAD>\n<style>\n  img { display: block;  margin-left: 5px;  margin-right: auto;  max-width: 100%;} \n @media print {img {page-break-after: always;}} \n </style></HEAD>\n<BODY>\n'

    # Itera sobre os arquivos .svg e adiciona as tags correspondentes ao HTML
    for i, svg_file in enumerate(svgs, start=1):
        svg_filename = os.path.basename(svg_file)
        html += f'<H1>[{i}] {svg_filename}</H1>\n'
        html += f'<IMG SRC="{svg_filename}" NAME="{svg_filename}" />\n'

    # Fecha as tags HTML
    html += '</BODY>\n</HTML>'

    # Define o caminho completo para o arquivo HTML na pasta atual
    html_file_path = os.path.join(svg_directory, "output.html")

    # Salva o conteúdo HTML no arquivo
    with open(html_file_path, "w") as file:
        file.write(html)

# Chama a função para gerar o HTML
svg_directory = input("Informe o caminho da pasta que contém os arquivos .svg: ")
generate_html(svg_directory)
