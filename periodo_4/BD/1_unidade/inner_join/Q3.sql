WITH Aluno AS (
	SELECT
		usuario.primeiro_nome, usuario.sobrenome, estudante.mat_estudante
	FROM
		universidade.estudante
		JOIN
			universidade.usuario ON usuario.cpf = estudante.cpf
	order by
		estudante.mat_estudante DESC
)
SELECT
	aluno.primeiro_nome aluno_primeiro_nome, aluno.sobrenome aluno_sobrenome,
	prof.primeiro_nome orientador_primeiro_nome, prof.sobrenome orientador_sobrenome
FROM
	Aluno aluno
	JOIN
		universidade.plano ON plano.mat_estudante = aluno.mat_estudante
	JOIN
		universidade.professor ON plano.mat_professor = professor.mat_professor
	JOIN
		universidade.usuario prof ON prof.cpf = professor.cpf
