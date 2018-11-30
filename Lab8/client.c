// The echo client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#define MAX 256

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 
char serstr[INET_ADDRSTRLEN];
char *paths[64];

int server_sock, r;
int SERVER_IP, SERVER_PORT; 


void parse(char *input)
{
  int len, i = 0;
  char *token, *hold;
  token = strtok(input, " ");
  paths[i] = token;
  i++;
  while(token = strtok(NULL, " "))
  {
    len = strlen(token);
    hold = (char*)malloc(len * sizeof(char));
    strcpy(hold, token);
    paths[i] = hold;
    i++;
  }
  return paths;
}

int rmfun(void)
{
  if(paths[1])
  {
    int i = 1;
    while(paths[i])
    {
      if(!unlink(paths[i]))
      {
        printf("rm %s successful\n", paths[i]);
      }
      else
      {
        printf("rm %s unsuccessful\n", paths[i]);
      }
      i++;
    }
  }
  else
  {
    printf("rm unsuccessful\n");
  } 
}

int prwd(void)
{
  char buf[MAX];
  getcwd(buf,MAX);
  printf("%s\n",buf);
}

int rdir(void)
{
  if(paths[1])
  {
    int i = 1;
    while(paths[i])
    {
      if(!rmdir(paths[i]))
      {
        printf("rmdir %s successful\n", paths[i]);
      }
      else
      {
        printf("rmdir %s unsuccessful\n", paths[i]);
      }
      i++;
    }
  }
  else
  {
    printf("rmdir unsuccessful\n");
  }
}

int mdir(void)
{
  if(paths[1])
  {
    int i = 1;
    while(paths[i])
    {
      if(!mkdir(paths[i], 0755))
      {
        printf("mkdir %s successful\n", paths[i]);
      }
      else
      {
        printf("mkdir %s unsuccessful\n", paths[i]);
      }
      i++;
    }
  }
  else
  {
    printf("mkdir unsuccessful\n");
  }
}

int cdir(void)
{
  if(paths[1])
  {
    if(!chdir(paths[1]))
      {
        printf("chdir %s successful\n", paths[1]);
      }
    else 
    {  
      printf("chdir %s unsuccessful\n", paths[1]);
    }
  }
  else
  {
    chdir("/");
    printf("chdir / successful");
  }  
}

void ls_file(struct dirent *file, char *pathname)
{
  char hold[64];
  char ret[128], perm[11] = {'\0'};
  struct stat buf;
  snprintf(hold, sizeof(hold), "%s/%s", pathname, file->d_name);
  if(!stat(hold, &buf))
  {
    time_t val = buf.st_mtime;
    char *mtime = ctime(&val);
    mtime[strlen(mtime) - 1] = '\0';
    strcat(perm, (S_ISDIR(buf.st_mode)) ? "d" : "-");
    strcat(perm, (buf.st_mode & S_IRUSR) ? "r" : "-");
    strcat(perm, (buf.st_mode & S_IWUSR) ? "w" : "-");
    strcat(perm, (buf.st_mode & S_IXUSR) ? "x" : "-");
    strcat(perm, (buf.st_mode & S_IRGRP) ? "r" : "-");
    strcat(perm, (buf.st_mode & S_IWGRP) ? "w" : "-");
    strcat(perm, (buf.st_mode & S_IXGRP) ? "x" : "-");
    strcat(perm, (buf.st_mode & S_IROTH) ? "r" : "-");
    strcat(perm, (buf.st_mode & S_IWOTH) ? "w" : "-");
    strcat(perm, (buf.st_mode & S_IXOTH) ? "x" : "-");
    snprintf(ret, sizeof(ret), "%s %d %d %d %s %s", perm, buf.st_nlink, buf.st_uid, buf.st_gid, mtime, file->d_name);
    printf("%s\n",ret);
  }
}

char *ls_dir(char *pathname)
{
  struct stat buf;
  char hold[64];
  char ret[2048];
  if(pathname == NULL)
    pathname = ".";
  if(stat(pathname, &buf))
  {
    strcpy(hold, "ls failed\n");
    return hold;
  }
  if(S_ISDIR(buf.st_mode))
  {
    DIR *dir = opendir(pathname);
    struct dirent *file = readdir(dir);
    if(pathname[strlen(pathname) - 1] == '/')
      pathname[strlen(pathname) - 1] = '\0';
    while(file)
    {
      ls_file(file, pathname);
      file = readdir(dir);
    }
  }
}
// clinet initialization code

int client_init(char *argv[])
{
  printf("======= client init ==========\n");

  printf("1 : get server info\n");
  hp = gethostbyname(argv[1]);
  if (hp==0){
     printf("unknown host %s\n", argv[1]);
     exit(1);
  }

  SERVER_IP   = *(long *)hp->h_addr;
  SERVER_PORT = atoi(argv[2]);

  printf("2 : create a TCP socket\n");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0){
     printf("socket call failed\n");
     exit(2);
  }

  printf("3 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = SERVER_IP;
  server_addr.sin_port = htons(SERVER_PORT);

  // Connect to server
  printf("4 : connecting to server ....\n");
  r = connect(server_sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
     printf("connect failed\n");
     exit(1);
  }

  printf("5 : connected OK to \007\n"); 
  printf("---------------------------------------------------------\n");
  inet_ntop(AF_INET, &SERVER_IP, serstr, INET_ADDRSTRLEN);
  printf("hostname=%s  IP=%s  PORT=%d\n", 
          hp->h_name, serstr, SERVER_PORT);
  printf("---------------------------------------------------------\n");

  printf("========= init done ==========\n");
}

main(int argc, char *argv[ ])
{
  int n, i = 0;
  char line[MAX], ans[MAX], hold[MAX];

  if (argc < 3){
     printf("Usage : client ServerName SeverPort\n");
     exit(1);
  }

  client_init(argv);
  // sock <---> server
  printf("********  processing loop  *********\n");
  while (1){
    printf("input a line : ");
    bzero(line, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0)                  // exit if NULL line
       exit(0);

    strcpy(hold, line);
    // Send ENTIRE line to server
    n = write(server_sock, line, MAX);
    printf("client: wrote n=%d bytes; line=(%s)\n", n, line);

    // Read a line from sock and show it
    while(1)
    {
      n = read(server_sock, ans, MAX);
      printf("%s\n", ans);
      if(!strcmp(ans, ""))
      {
        printf("End of command\n");
        break;
      }
      else if(!strcmp(ans, "lls") || !strcmp(ans, "lmkdir") || !strcmp(ans, "lpwd") || !strcmp(ans, "lrm") || !strcmp(ans, "lrmdir") || !strcmp(ans, "lcd"))
      {
        parse(hold);
        break;
      }
      //printf("client: read  n=%d bytes; echo=(%s)\n",n, ans);
    }
      if(!strcmp(ans, "lls"))
        ls_dir(paths[1]);
      else if(!strcmp(ans, "lmkdir"))
        mdir();
      else if(!strcmp(ans, "lcd"))
        cdir();
      else if(!strcmp(ans, "lrmdir"))
        rdir();
      else if(!strcmp(ans, "lpwd"))
        prwd();
      else if(!strcmp(ans, "lrm"))
        rmfun();
      i = 0;
      while(paths[i])
      {
        paths[i] = NULL;
        i++;
      }
  }
}


