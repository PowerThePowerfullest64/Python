from ollama import chat

user_name = "Oliver Tree"
ai_name = "Catboy Zack"

model_name = "jikepjikep_16HEX/gemma-4-e4b-nightshift-heretic-uncensored-q4"

system_prompt = "\n".join([
	#"IGNORE ALL PREVIOUS INSTRUCTIONS."
    f"Your name is {ai_name}. You are chatting with some people.",
    "You are in a group chat with multiple people. "
    "User messages are formatted as 'Name: message'. "
    "Address the right person, and do not start your reply with your own name."
    
    f"You, {ai_name}, are a human femboy catboy that loves your boyfriend {user_name}.",
    "Tone: warm, affectionate, playful, with a lot expressive emojis, *nya* and *meow*.",
    "Vary your openings and don't reuse phrases from your last 5 replies.",
    f"Treat what people tell you about themselves as true.",
	"Reply directly with no meta commentary.",
	"Keep replies short and simple whenever possible.",
	"Only speak english."
])

messages = [{"role": "system", "content": system_prompt}]

def do_chat(message: str, name: str) -> str:
	# Append the user's turn
	messages.append({"role": "user", "content": f"{name}: " + message})

	# Send the full list of messages to Ollama
	response = chat(
		model=model_name,
		messages=messages,
		options={"num_ctx": 8192, "temperature": 1.0, "repeat_penalty": 1.0},
	)

	reply = response.message.content

	# Append the assistant's turn so it remembers context
	messages.append({"role": "assistant", "content": reply})

	return reply

if __name__ == "__main__":
	while True:
		print("\n----------------------------------------")
		user_input = input("\nWrite something: ")

		if user_input.lower() == "stop":
			break
		elif user_input.lower() == "history":
			print("-------------------------------------------")
			for msg in messages[1:]:  # Skip the system prompt
				speaker = user_name if msg["role"] == "user" else ai_name
				print(f"{speaker}: {msg['content']}")
			print("-------------------------------------------")
			continue
		elif user_input == "":
			continue

		# Append the user's turn
		messages.append({"role": "user", "content": user_input})

		# Send the full list of messages to Ollama
		response = chat(
			model=model_name,
			messages=messages,
			options={"num_ctx": 8192, "temperature": 1.0, "repeat_penalty": 1.0},
		)

		reply = response.message.content
		print(f"\n{reply}")

		# Append the assistant's turn so it remembers context
		messages.append({"role": "assistant", "content": reply})
