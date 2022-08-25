WITH Aluno AS (
	SELECT
		usuario.primeiro_nome, usuario.sobrenome, estudante.mat_estudante
	FROM
		universidade.estudante
	JOIN
		universidade.usuario ON usuario.cpf = estudante.cpf
	order by
		estudante.mat_estudante
	LIMIT
		1
)
SELECT
	primeiro_nome, sobrenome, nome
FROM
	Aluno
JOIN
	universidade.cursa ON cursa.mat_estudante = Aluno.mat_estudante
JOIN 
	universidade.turma ON turma.id_turma = cursa.id_turma
JOIN
	universidade.disciplina ON disciplina.cod_disc = turma.cod_disc
