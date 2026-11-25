ЛР7 — Семисал Гліб, група 80, варіант 22.
Інструкція для двох ПК або двох Linux VM (Ubuntu/Debian).
На обох ПК: sudo apt install openssh-server openssh-client cron
sudo systemctl enable --now ssh cron
ip addr show
systemctl status ssh --no-pager
Замість USER1, USER2, IP1, IP2 підставити реальні значення.
PC2: ssh USER1@IP1
pwd
touch ~/test_pc2.txt
exit
PC1: ls -l ~/test_pc2.txt
sudo journalctl -u ssh --no-pager -n 20
PC1: ssh USER2@IP2
pwd
touch ~/test_pc1.txt
exit
PC2: sudo journalctl -u ssh --no-pager -n 20

PC1: ssh-keygen -t rsa -b 3072
Не перезаписувати наявний ключ. За потреби обрати окремий файл.
scp ~/.ssh/id_rsa.pub USER2@IP2:/tmp/pc1_key.pub
PC2: mkdir -p ~/.ssh; chmod 700 ~/.ssh
cat /tmp/pc1_key.pub >> ~/.ssh/authorized_keys
chmod 600 ~/.ssh/authorized_keys
cat ~/.ssh/authorized_keys
PC1: ssh -o BatchMode=yes USER2@IP2 'pwd'
echo 'Test from PC1' > ~/pc1_test.txt
scp ~/pc1_test.txt USER2@IP2:~/
PC2: cat ~/pc1_test.txt
mkdir -p ~/pc2_dir
echo 'Test from PC2' > ~/pc2_dir/pc2_test.txt
sudo journalctl -u ssh --no-pager -n 20
PC1: scp -r USER2@IP2:~/pc2_dir ~/
ls -l ~/pc2_dir; cat ~/pc2_dir/pc2_test.txt
sudo journalctl -u ssh --no-pager -n 20
Повторити обмін у зворотному напрямку, змінивши ролі та імена файлів.
Приватний ключ не копіювати та не додавати до GitHub.

Cron:
gcc -std=c11 -Wall -Wextra current_time.c -o current_time_80_22
sudo install -m 755 current_time_80_22 /usr/local/bin/current_time_80_22
systemctl status cron --no-pager
crontab -e
Додати рядок: */2 * * * * /usr/local/bin/current_time_80_22
crontab -l
Через 4–6 хвилин: cat ~/current_time_80_22.log
sudo journalctl -u cron --no-pager -n 30
Після перевірки видалити лише доданий рядок через crontab -e.
На Fedora назви служб sshd та crond.
