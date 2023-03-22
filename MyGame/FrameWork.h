#pragma once
class FrameWork
{
public: // �����o�֐�

	// ���s
	void Run();

	// ������
	virtual void Initialize();

	// �I��
	virtual void Finalize();

	// �X�V
	virtual void Update();

	// �`��
	virtual void Draw() = 0;

	// �I���`�F�b�N
	virtual bool IsEndRequest() { return isEndRequest; }
	
	virtual ~FrameWork() = default;

protected: //�����o�ϐ�
//�Q�[�����[�v�I�����N�G�X�g
	bool isEndRequest = false;
};

