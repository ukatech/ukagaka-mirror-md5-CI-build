# Attention!  
**disable autocrlf** when you use it  

______

# ukagaka-mirror-md5-CI-build  
 > How bad would it be if the md5 didn't match  

about `github action`: https://docs.github.com/actions  

useage: https://github.com/Taromati2/stable-mirror/blob/master/.github/workflows/md5-CI-build.yml  
```yml
#//...
runs-on: windows-latest
steps:
- name: disable auto crlf
  uses: steve02081504/disable-autocrlf@v1
  #//...
  #//checkout your ghost file: actions/checkout
  #//...
  #//dosome check like shiori-check or do nothing: Taromati2/yaya-CI-check
  #//...
- name: md5file-CI-build
  uses: Taromati2/ukagaka-mirror-md5-CI-build@v1
  with:
    # where is your update file?
    dir-path: .\Taromati2 # default is .
    # ghost_dir(for ghost & each shell) or other(only for a dir)
    build-mode: ghost_dir # default is ghost_dir
    # 
    # commit-message: # optional, default is 'md5 fix~'
    # 
    # committer-email: # optional, default is taromati2@foxmail.com
    # 
    # committer-name: # optional, default is Taromati2
```

# rules for ignore  
`md5buildignore.txt` for type `other` & `ghost_dir`'s ghost part  
`md5buildignoreforeach.txt` for `ghost_dir`'s shell part  

______

ignores `*.inf` under `/ghost/data/*`  
```
/ghost/data/*/*.inf
```
unignore `love.inf` under `/ghost/data/*`  
```
!/ghost/data/*/love.inf
```
ignore `some.log1`, `some.log2`, `some.logA`...(and others) under **ANY Folder**  
```
some.log?
```
or
```
*/some.log?
```

______

useage:  
https://github.com/Taromati2/stable-mirror/blob/master/Taromati2/md5buildignore.txt  
https://github.com/Taromati2/stable-mirror/blob/master/Taromati2/shell/md5buildignoreforeach.txt  

