SELECT 
	aluno.primeiro_nome aluno_primero_nome, aluno.sobrenome aluno_sobrenome,
	prof.primeiro_nome prof_primero_nome, prof.sobrenome prof_sobrenome
FROM 
	universidade.estudante 
JOIN
	universidade.usuario aluno ON aluno.cpf = estudante.cpf
LEFT OUTER JOIN
	universidade.plano ON plano.mat_estudante = estudante.mat_estudante 
FULL OUTER JOIN
	universidade.professor ON professor.mat_professor = plano.mat_professor
LEFT OUTER JOIN
	universidade.usuario prof ON prof.cpf = professor.cpf;
