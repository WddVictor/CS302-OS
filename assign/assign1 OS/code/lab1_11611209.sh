source_file="${1}";
target_file="${2}";
current_file="$(pwd)";

resolveParameter(){
	initqueue;
	cd "${source_file}";
	source_file="$(pwd)";
	
	filename="${source_file##*/}";
	enqueue "${filename}";

	cd $current_file;

	output_name="${target_file##*/}"
	output_path="${target_file%/*}";


	if [ "${output_name}" = "${output_path}" ]
		then
		output_path="";
	fi
	if [ ! -d "${output_path}" -a "${output_path}" != "" ]
		then
		mkdir -p "${output_path}";
	fi


	cd "${output_path}";
	rm -rf "${output_name}";
	touch "${output_name}";
	target_file="$(pwd)"/"$output_name";
	cd "current_file";

	# if [ ! -d "${target_file}" -a "${output_path}" != "" ]
	# 	then
	# 	rm -rf "${target_file}";
	# 	mkdir -p "${target_file}";

	# fi

	# cd "${target_file}";

	# target_file="$(pwd)";
	# target_file="${target_file}";
	# cd "$current_file";
	# touch "${target_file}";

	echo -n>"${target_file}";
}

initqueue(){
	header=0;
	rear=0;
}

enqueue(){ 
	queue=("${queue[@]}" "${1}")

	let rear++;
}

dequeue(){ 
	removed="${queue[header]}";
	let header++;
}

display(){
	dequeue;
	echo ["${removed}"]>>"${target_file}";
	cd "${1}";
	for name in *;do

		if [ "$(ls)" != "" ]
		then
			
			if [ -d "${name}" ]
			then
				let directory_counter++;
				cd "${name}";
				enqueue "$(pwd)";
				enqueue "${name}";
				cd ../;
			else
				let file_counter++;
			fi
			
			echo "${1}/""${name}">>"${target_file}"
		fi
	done
	echo>>"${target_file}";
}

directory_counter=0;
file_counter=0;

resolveParameter;


display "${source_file}";

while [ $header != $rear ]
do
	dequeue;
	display "${removed}";
done

echo [Directories Count]:$directory_counter>>"${target_file}";
echo [Files Count]:$file_counter>>"${target_file}";